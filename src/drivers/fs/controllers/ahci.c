/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#pragma GCC optimize ("O0")

#include <fs/controllers/ahci.h>
#include <types.h>
#include <string.h>
#include <memory.h>
#include <msg.h>
#include <pci.h>
#include <wait.h>
#include <panic.h>

// implicit declaration fix
int ahci_init_from_bar(uint64_t abar_phys);

static void print_hex_val(uint32_t val) {
    char buf[9];
    const char* hex = "0123456789ABCDEF";
    buf[8] = '\0';
    for(int i = 7; i >= 0; i--) {
        buf[i] = hex[val & 0xF];
        val >>= 4;
    }
    print(buf, 0x07);
}

static void print_dec_val(uint32_t val) {
    char buf[12];
    int i = 11;
    buf[i--] = '\0';
    
    if(val == 0) {
        buf[i--] = '0';
    } else {
        while(val > 0 && i >= 0) {
            buf[i--] = '0' + (val % 10);
            val /= 10;
        }
    }
    print(&buf[i+1], 0x07);
}

#define AHCI_LOG(x) print(x, 0x07)
#define AHCI_DBG(x) print(x, 0x07)
#define AHCI_ERR(x) print_error(x)

static ahci_controller_t g_ahci;
static ahci_port_t g_ports[AHCI_MAX_PORTS];

HBA_PORT* hba_port(HBA_MEM* hba, int portno) {
    return (HBA_PORT*)((uint8_t*)hba + 0x100 + (portno * 0x80));
}

static int wait_port_cmd(HBA_PORT *port, int target_state, unsigned int timeout_ms) {
    unsigned int waited = 0;
    while (waited < timeout_ms) {
        int current = (port->cmd & HBA_PxCMD_CR) ? 1 : 0;
        if (current == target_state) return 0;
        sleep_ms(1);
        waited++;
    }
    AHCI_ERR("wait_port_cmd: timeout");
    return -1;
}

static int stop_port(ahci_port_t *p) {
    HBA_PORT volatile *port = p->port;
    
    AHCI_DBG("Stopping port...\n");
    
    port->cmd &= ~HBA_PxCMD_ST;
    
    unsigned int timeout = 500;
    while (timeout-- > 0) {
        if (!(port->cmd & HBA_PxCMD_FR) && !(port->cmd & HBA_PxCMD_CR)) {
            break;
        }
        sleep_ms(1);
    }
    
    if (port->cmd & (HBA_PxCMD_FR | HBA_PxCMD_CR)) {
        AHCI_ERR("stop_port: FR or CR still set\n");
        return -1;
    }
    
    port->cmd &= ~HBA_PxCMD_FRE;
    
    AHCI_DBG("Port stopped\n");
    return 0;
}

static int start_port(ahci_port_t *p) {
    HBA_PORT volatile *port = p->port;
    
    AHCI_DBG("Starting port...\n");
    
    port->cmd |= HBA_PxCMD_FRE;
    
    unsigned int timeout = 500;
    while (timeout-- > 0) {
        if (port->cmd & HBA_PxCMD_FR) {
            break;
        }
        sleep_ms(1);
    }
    
    if (!(port->cmd & HBA_PxCMD_FR)) {
        AHCI_ERR("start_port: FR not set\n");
        return -1;
    }

    port->cmd |= HBA_PxCMD_ST;
    
    AHCI_DBG("Port started\n");
    return 0;
}

static int find_cmdslot(HBA_PORT *port) {
    uint32_t slots = port->sact | port->ci;
    for (int i = 0; i < AHCI_MAX_CMD_SLOTS; i++) {
        if (!(slots & (1u << i))) return i;
    }
    AHCI_ERR("No free command slot\n");
    return -1;
}

static int port_rebase(ahci_port_t *p) {
    HBA_PORT volatile *port = p->port;
    
    AHCI_DBG("Rebasing port...\n");
    
    if (stop_port(p) < 0) {
        AHCI_ERR("port_rebase: failed to stop port\n");
        return -1;
    }

    size_t fis_size = 256;
    uint64_t fis_phys = phys_alloc_contiguous(fis_size, 256);
    if (!fis_phys) {
        AHCI_ERR("port_rebase: failed to alloc FIS\n");
        return -1;
    }
    void *fis_virt = (void*)(uintptr_t)fis_phys;
    memset(fis_virt, 0, fis_size);

    port->fb = (uint32_t)(fis_phys & 0xFFFFFFFF);
    port->fbu = (uint32_t)((fis_phys >> 32) & 0xFFFFFFFF);
    p->fb_phys = fis_phys;
    p->fb_virt = fis_virt;
    
    AHCI_DBG("FIS allocated\n");

    size_t clb_size = 1024;
    uint64_t clb_phys = phys_alloc_contiguous(clb_size, 1024);
    if (!clb_phys) { 
        AHCI_ERR("port_rebase: failed to alloc CLB\n"); 
        phys_free_contiguous(fis_phys, fis_size);
        return -1; 
    }
    void *clb_virt = (void*)(uintptr_t)clb_phys;
    memset(clb_virt, 0, clb_size);

    port->clb = (uint32_t)(clb_phys & 0xFFFFFFFF);
    port->clbu = (uint32_t)((clb_phys >> 32) & 0xFFFFFFFF);
    p->clb_phys = clb_phys;
    p->clb_virt = clb_virt;
    
    AHCI_DBG("CLB allocated\n");

    HBA_CMD_HEADER *cmdheaders = (HBA_CMD_HEADER *)clb_virt;
    for (int i = 0; i < AHCI_MAX_CMD_SLOTS; i++) {
        size_t tbl_size = sizeof(HBA_CMD_TBL);
        uint64_t tbl_phys = phys_alloc_contiguous(tbl_size, 128);
        if (!tbl_phys) { 
            AHCI_ERR("port_rebase: failed to alloc cmd_tbl\n"); 
            for (int j = 0; j < i; j++) {
                phys_free_contiguous(p->cmd_tbl_phys[j], tbl_size);
            }
            phys_free_contiguous(clb_phys, clb_size);
            phys_free_contiguous(fis_phys, fis_size);
            return -1; 
        }
        void *tbl_virt = (void*)(uintptr_t)tbl_phys;
        memset(tbl_virt, 0, tbl_size);
        p->cmd_tbl_phys[i] = tbl_phys;
        p->cmd_tbl_virt[i] = tbl_virt;

        cmdheaders[i].prdtl = 0;
        cmdheaders[i].ctba = (uint32_t)(tbl_phys & 0xFFFFFFFF);
        cmdheaders[i].ctbau = (uint32_t)((tbl_phys >> 32) & 0xFFFFFFFF);
    }
    
    AHCI_DBG("Command tables allocated\n");

    port->is = (uint32_t)-1;
    port->ie = 0xFFFFFFFF;   

    if (start_port(p) < 0) {
        AHCI_ERR("port_rebase: failed to start port\n");
        return -1;
    }
    
    AHCI_DBG("Port rebased successfully\n");
    return 0;
}

static int port_device_present(HBA_PORT *port) {
    uint32_t ssts = port->ssts;
    uint32_t det = ssts & 0x0F;
    uint32_t ipm = (ssts >> 8) & 0x0F;
    
    AHCI_DBG("Checking device presence: DET=0x");
    print_hex_val(det);
    AHCI_DBG(", IPM=0x");
    print_hex_val(ipm);
    AHCI_DBG("\n");
    
    if (det != 0x3) {
        AHCI_DBG("Device not present (DET != 3)\n");
        return 0;
    }
    
    AHCI_DBG("Device present\n");
    return 1;
}

static void build_reg_h2d(uint8_t *cfis, uint8_t command, uint8_t features, uint32_t lba, uint16_t count) {
    memset(cfis, 0, 64);
    cfis[0] = FIS_TYPE_REG_H2D;
    cfis[1] = 1 << 7;
    cfis[2] = command;
    cfis[3] = features;
    cfis[4] = (uint8_t)(lba & 0xFF);
    cfis[5] = (uint8_t)((lba >> 8) & 0xFF);
    cfis[6] = (uint8_t)((lba >> 16) & 0xFF);
    cfis[7] = (1 << 6); 
    cfis[8] = (uint8_t)(count & 0xFF);
    cfis[9] = (uint8_t)((count >> 8) & 0xFF);
}

static int wait_for_completion(HBA_PORT *port, int slot, unsigned int timeout_ms) {
    unsigned int waited = 0;
    while (waited < timeout_ms) {
        if (!(port->ci & (1u << slot))) {
            uint32_t tfd = port->tfd;
            if (tfd & (1 << 0)) { 
                AHCI_ERR("Device error: TFD=0x");
                print_hex_val(tfd);
                AHCI_ERR("\n");
                return -1;
            }
            return 0; 
        }
        
        if (port->tfd & (0x88)) {
            AHCI_ERR("Fatal error during command: TFD=0x");
            print_hex_val(port->tfd);
            AHCI_ERR("\n");
            return -1;
        }
        
        sleep_ms(1);
        waited++;
    }
    
    AHCI_ERR("Command timeout after ");
    print_dec_val(timeout_ms);
    AHCI_ERR(" ms\n");
    
    port->ci &= ~(1u << slot);
    return -1;
}

static int port_identify(ahci_port_t *p, uint64_t buffer_phys) {
    HBA_PORT *port = p->port;
    
    AHCI_DBG("Sending IDENTIFY command...\n");
    
    int slot = find_cmdslot(port);
    if (slot < 0) { 
        AHCI_ERR("identify: no free slot\n"); 
        return -1; 
    }
    
    AHCI_DBG("Using command slot ");
    print_dec_val(slot);
    AHCI_DBG("\n");

    HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER *)((uintptr_t)p->clb_virt + (slot * sizeof(HBA_CMD_HEADER)));
    memset(cmdheader, 0, sizeof(HBA_CMD_HEADER));

    HBA_CMD_TBL *cmdtbl = (HBA_CMD_TBL *)p->cmd_tbl_virt[slot];
    memset(cmdtbl, 0, sizeof(HBA_CMD_TBL));

    cmdheader->cfl = 5; 
    cmdheader->w = 0;  
    cmdheader->prdtl = 1;

    HBA_PRDT_ENTRY *prdt = &cmdtbl->prdt_entry[0];
    prdt->dba = (uint32_t)(buffer_phys & 0xFFFFFFFF);
    prdt->dbau = (uint32_t)((buffer_phys >> 32) & 0xFFFFFFFF);
    prdt->dbc = 511; 

    build_reg_h2d(cmdtbl->cfis, 0xEC, 0, 0, 0);
    
    AHCI_DBG("Sending command...\n");
    
    port->ci = (1u << slot);

    int result = wait_for_completion(port, slot, 5000);
    
    if (result == 0) {
        AHCI_DBG("IDENTIFY completed successfully\n");
    } else {
        AHCI_ERR("IDENTIFY failed\n");
    }
    
    return result;
}

static int port_read_single(ahci_port_t *p, uint64_t buffer_phys, uint64_t start_lba) {
    HBA_PORT volatile *port = p->port;
    
    AHCI_DBG("Reading sector ");
    print_dec_val(start_lba);
    AHCI_DBG("\n");
    
    int slot = find_cmdslot(port);
    if (slot < 0) { 
        AHCI_ERR("read: no free slot\n"); 
        return -1; 
    }

    HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER *)((uintptr_t)p->clb_virt + (slot * sizeof(HBA_CMD_HEADER)));
    memset(cmdheader, 0, sizeof(HBA_CMD_HEADER));

    HBA_CMD_TBL *cmdtbl = (HBA_CMD_TBL *)p->cmd_tbl_virt[slot];
    memset(cmdtbl, 0, sizeof(HBA_CMD_TBL));

    cmdheader->cfl = 5;
    cmdheader->w = 0; 
    cmdheader->prdtl = 1;

    HBA_PRDT_ENTRY *prdt = &cmdtbl->prdt_entry[0];
    prdt->dba = (uint32_t)(buffer_phys & 0xFFFFFFFF);
    prdt->dbau = (uint32_t)((buffer_phys >> 32) & 0xFFFFFFFF);
    prdt->dbc = 511; 

    memset(cmdtbl->cfis, 0, 64);
    cmdtbl->cfis[0] = FIS_TYPE_REG_H2D;
    cmdtbl->cfis[1] = 1 << 7;
    cmdtbl->cfis[2] = 0x25;  
    
    if (start_lba < 0x10000000) { 
        cmdtbl->cfis[3] = 0;
        cmdtbl->cfis[4] = (uint8_t)(start_lba & 0xFF);
        cmdtbl->cfis[5] = (uint8_t)((start_lba >> 8) & 0xFF);
        cmdtbl->cfis[6] = (uint8_t)((start_lba >> 16) & 0xFF);
        cmdtbl->cfis[7] = (1 << 6); 
        cmdtbl->cfis[8] = 1;   
    } else { 
        cmdtbl->cfis[3] = 0;
        cmdtbl->cfis[4] = (uint8_t)(start_lba & 0xFF);
        cmdtbl->cfis[5] = (uint8_t)((start_lba >> 8) & 0xFF);
        cmdtbl->cfis[6] = (uint8_t)((start_lba >> 16) & 0xFF);
        cmdtbl->cfis[7] = (1 << 6);
        cmdtbl->cfis[8] = 1;
        cmdtbl->cfis[9] = 0;
        cmdtbl->cfis[10] = (uint8_t)((start_lba >> 24) & 0xFF);
        cmdtbl->cfis[11] = 0;
        cmdtbl->cfis[12] = 0;
    }

    AHCI_DBG("Issuing read command...\n");
    
    port->ci = (1u << slot);

    int result = wait_for_completion(port, slot, 5000);
    
    if (result == 0) {
        AHCI_DBG("Read completed successfully\n");
    } else {
        AHCI_ERR("Read failed\n");
    }
    
    return result;
}

int ahci_init_from_pci(uint8_t bus, uint8_t slot, uint8_t func) {
    AHCI_LOG("Initializing AHCI from PCI...");
    
    uint64_t bar0 = pci_get_bar64(bus, slot, func, 5); 
    if (!bar0) {
        AHCI_ERR("ahci_init_from_pci: no BAR found\n");
        return -1;
    }
    
    AHCI_DBG("AHCI BAR: 0x");
    print_hex_val(bar0);
    AHCI_DBG("\n");
    
    return ahci_init_from_bar(bar0);
}

int ahci_init_from_bar(uint64_t bar_phys) {
    AHCI_LOG("Initializing AHCI from BAR...");
    
    void *hba_virt = (void*)(uintptr_t)bar_phys;
    HBA_MEM *hba = (HBA_MEM *)hba_virt;
    
    AHCI_DBG("HBA memory at: 0x");
    print_hex_val((uintptr_t)hba);
    AHCI_DBG("\n");
    
    if (!(hba->cap & (1 << 31))) {
        AHCI_ERR("AHCI not supported by controller\n");
        return -1;
    }
    
    g_ahci.hba = hba;
    g_ahci.hba_phys = bar_phys;
    g_ahci.ports_implemented = hba->pi;
    
    AHCI_DBG("Ports implemented: 0x");
    print_hex_val(hba->pi);
    AHCI_DBG("\n");
    
    hba->ghc |= HBA_GHC_AE;
    
    unsigned int timeout = 1000;
    while (timeout-- > 0) {
        if (hba->ghc & HBA_GHC_AE) {
            break;
        }
        sleep_ms(1);
    }
    
    if (!(hba->ghc & HBA_GHC_AE)) {
        AHCI_ERR("Failed to enable AHCI mode\n");
        return -1;
    }
    
    AHCI_DBG("AHCI mode enabled\n");

    int count = 0;
    for (int i = 0; i < AHCI_MAX_PORTS; i++) {
        if (hba->pi & (1u << i)) {
            count++;
            AHCI_DBG("Port ");
            print_dec_val(i);
            AHCI_DBG(" implemented\n");
        }
    }
    g_ahci.port_count = count;

    for (int i = 0; i < AHCI_MAX_PORTS; i++) {
        if (!(hba->pi & (1u << i))) continue;
        
        AHCI_LOG("Initializing port ");
        print_dec_val(i);
        AHCI_LOG("\n");
        
        ahci_port_t *p = &g_ports[i];
        memset(p, 0, sizeof(ahci_port_t));
        p->port_no = i;
        p->port = hba_port(hba, i);

        if (!port_device_present(p->port)) {
            AHCI_DBG("No device on port ");
            print_dec_val(i);
            AHCI_DBG("\n");
            continue;
        }

        if (port_rebase(p) < 0) {
            AHCI_ERR("Failed to rebase port ");
            print_dec_val(i);
            AHCI_ERR("\n");
            continue;
        }

        AHCI_DBG("Sending IDENTIFY to port ");
        print_dec_val(i);
        AHCI_DBG("\n");
        
        uint64_t ident_phys = phys_alloc_contiguous(512, 512);
        if (!ident_phys) {
            AHCI_ERR("Failed to allocate IDENTIFY buffer\n");
            continue;
        }
        
        void *ident_virt = (void*)(uintptr_t)ident_phys;
        memset(ident_virt, 0, 512);
        
        if (port_identify(p, ident_phys) == 0) {
            AHCI_LOG("IDENTIFY successful on port ");
            print_dec_val(i);
            AHCI_LOG("\n");

            uint64_t read_phys = phys_alloc_contiguous(512, 512);
            if (read_phys) {
                void *read_virt = (void*)(uintptr_t)read_phys;
                memset(read_virt, 0, 512);
                
                if (port_read_single(p, read_phys, 0) == 0) {
                    AHCI_LOG("Read successful on port ");
                    print_dec_val(i);
                    AHCI_LOG("\n");
                } else {
                    AHCI_ERR("Read failed on port ");
                    print_dec_val(i);
                    AHCI_ERR("\n");
                }
                
                phys_free_contiguous(read_phys, 512);
            }
        } else {
            AHCI_ERR("IDENTIFY failed on port ");
            print_dec_val(i);
            AHCI_ERR("\n");
        }
        
        phys_free_contiguous(ident_phys, 512);
    }

    AHCI_LOG("AHCI initialization complete\n");
    return 0;
}

pci_device_t find_ahci_controller() {
    AHCI_LOG("Searching for AHCI controller...\n");

    for (int bus = 0; bus < 256; bus++) {
        if ((bus % 16) == 0) {
            AHCI_DBG("Scanning bus ");
            print_dec_val(bus);
            AHCI_DBG("...\n");
        }
        
        for (uint8_t slot = 0; slot < 32; slot++) {
            for (uint8_t func = 0; func < 8; func++) {
                if (func > 0) {
                    uint16_t vendor = pci_config_read16(bus, slot, 0, 0x00);
                    if (vendor == 0xFFFF) break; 
                    
                    uint8_t header = pci_read_config8(bus, slot, 0, 0x0E);
                    if (!(header & 0x80)) break; 
                }
                
                uint16_t vendor = pci_config_read16(bus, slot, func, 0x00);
                if (vendor == 0xFFFF) continue;
                
                static int device_count = 0;
                device_count++;
                if ((device_count % 64) == 0) {
                    AHCI_DBG("Scanned ");
                    print_dec_val(device_count);
                    AHCI_DBG(" devices...\n");
                }
                
                uint8_t class = pci_read_config8(bus, slot, func, 0x0B);
                uint8_t subclass = pci_read_config8(bus, slot, func, 0x0A);
                
                if (vendor != 0xFFFF) {
                    AHCI_DBG("Found device: bus=");
                    print_dec_val(bus);
                    AHCI_DBG(" slot=");
                    print_dec_val(slot);
                    AHCI_DBG(" func=");
                    print_dec_val(func);
                    AHCI_DBG(" vendor=0x");
                    print_hex_val(vendor);
                    AHCI_DBG(" class=0x");
                    print_hex_val(class);
                    AHCI_DBG(" subclass=0x");
                    print_hex_val(subclass);
                    AHCI_DBG("\n");
                }
                
                if (class == 0x01 && subclass == 0x06) { 
                    pci_device_t dev;
                    dev.bus = bus;
                    dev.slot = slot;
                    dev.function = func;
                    dev.vendor_id = vendor;
                    dev.device_id = pci_config_read16(bus, slot, func, 0x02);
                    dev.class_id = class;
                    dev.subclass_id = subclass;
                    dev.prog_if = pci_read_config8(bus, slot, func, 0x09);
                    dev.header_type = pci_read_config8(bus, slot, func, 0x0E);

                    for (int i = 0; i < 6; i++)
                        dev.bar[i] = pci_config_read32(bus, slot, func, 0x10 + i*4);

                    AHCI_LOG("Found AHCI controller at ");
                    print("bus:slot.func = ", 0x07);
                    print_dec_val(bus);
                    print(":", 0x07);
                    print_dec_val(slot);
                    print(".", 0x07);
                    print_dec_val(func);
                    print("\n", 0x07);
                    
                    AHCI_DBG("Vendor: 0x");
                    print_hex_val(vendor);
                    AHCI_DBG(", Device: 0x");
                    print_hex_val(dev.device_id);
                    AHCI_DBG("\n");
                    
                    return dev;
                }
            }
        }
    }

    AHCI_LOG("No AHCI controller found after scanning all buses\n");

    kernelpanic("AHCI_NOT_FOUND", "Please ensure you are running AurorOS on a machine\n that supports AHCI. On QEMU add this flag: -device ahci,id=ahci0");

    pci_device_t empty = { .bus = 0xFF, .slot = 0xFF, .function = 0xFF };
    return empty;
}

void ahci_init() {
    AHCI_LOG("Starting AHCI initialization...\n");
    
    pci_device_t ahci_dev = find_ahci_controller();
    if (ahci_dev.bus != 0xFF) {
        AHCI_LOG("AHCI controller found, initializing...\n");
        ahci_init_from_pci(ahci_dev.bus, ahci_dev.slot, ahci_dev.function);
    } else {
        print_warn("No AHCI controller found!\n");
    }
}