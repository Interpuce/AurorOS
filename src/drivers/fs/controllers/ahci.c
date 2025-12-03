/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <fs/controllers/ahci.h>
#include <types.h>
#include <string.h>
#include <memory.h>
#include <msg.h>
#include <pci.h>
#include <wait.h>

// implicit declaration fix
int ahci_init_from_bar(uint64_t abar_phys);

// in-file macros
#define AHCI_LOG(x) print_info(x)

static ahci_controller_t g_ahci;
static ahci_port_t g_ports[AHCI_MAX_PORTS];

HBA_PORT* hba_port(HBA_MEM* hba, int portno) {
    return (HBA_PORT*)((uint8_t*)hba + 0x100 + (portno * 0x80));
}

static int wait_port_cmd(HBA_PORT *port, int start, unsigned int timeout_ms) {
    unsigned int waited = 0;
    while (1) {
        uint32_t cmd = port->cmd;
        int running = (cmd & HBA_PxCMD_CR) ? 1 : 0;
        if (start) {
            if (running) return 0;
        } else {
            if (!running) return 0;
        }
        if (waited >= timeout_ms) return -1;
        sleep_ms(1);
        waited++;
    }
}

static int stop_port(ahci_port_t *p) {
    HBA_PORT *port = p->port;
    port->cmd &= ~HBA_PxCMD_ST;
    port->cmd &= ~HBA_PxCMD_FRE;
    if (wait_port_cmd(port, 0, 500) < 0) {
        AHCI_LOG("stop_port: timeout waiting for CR to clear");
        return -1;
    }
    return 0;
}

static int start_port(ahci_port_t *p) {
    HBA_PORT *port = p->port;
    port->cmd |= HBA_PxCMD_FRE;
    port->cmd |= HBA_PxCMD_ST;
    if (wait_port_cmd(port, 1, 500) < 0) {
        AHCI_LOG("start_port: timeout waiting for CR to set");
        return -1;
    }
    return 0;
}

static int find_cmdslot(HBA_PORT *port) {
    uint32_t slots = port->sact | port->ci;
    for (int i = 0; i < AHCI_MAX_CMD_SLOTS; i++) {
        if (!(slots & (1u << i))) return i;
    }
    return -1;
}

static int port_rebase(ahci_controller_t *ctl, ahci_port_t *p) {
    HBA_PORT *port = p->port;
    if (stop_port(p) < 0) return -1;

    size_t fis_size = 256;
    uint64_t fis_phys = phys_alloc_contiguous(fis_size, 256);
    if (!fis_phys) {
        AHCI_LOG("port_rebase: failed to alloc FIS");
        return -1;
    }
    void *fis_virt = (void*)(uintptr_t)fis_phys;
    if (!fis_virt) { AHCI_LOG("port_rebase: virt_from_phys returned NULL for FIS"); return -1; }
    memset(fis_virt, 0, fis_size);
    port->fb = (uint32_t)(fis_phys & 0xFFFFFFFF);
    port->fbu = (uint32_t)((fis_phys >> 32) & 0xFFFFFFFF);
    p->fb_phys = fis_phys;
    p->fb_virt = fis_virt;

    size_t clb_size = 1024;
    uint64_t clb_phys = phys_alloc_contiguous(clb_size, 1024);
    if (!clb_phys) { AHCI_LOG("port_rebase: failed to alloc CLB"); return -1; }
    void *clb_virt = (void*)(uintptr_t)clb_phys;
    if (!clb_virt) { AHCI_LOG("port_rebase: virt_from_phys returned NULL for CLB"); return -1; }
    memset(clb_virt, 0, clb_size);
    port->clb = (uint32_t)(clb_phys & 0xFFFFFFFF);
    port->clbu = (uint32_t)((clb_phys >> 32) & 0xFFFFFFFF);
    p->clb_phys = clb_phys;
    p->clb_virt = clb_virt;

    for (int i = 0; i < AHCI_MAX_CMD_SLOTS; i++) {
        size_t tbl_size = 256;
        uint64_t tbl_phys = phys_alloc_contiguous(tbl_size, 128);
        if (!tbl_phys) { AHCI_LOG("port_rebase: failed to alloc cmd_tbl"); return -1; }
        void *tbl_virt = (void*)(uintptr_t)tbl_phys;
        if (!tbl_virt) { AHCI_LOG("port_rebase: virt_from_phys returned NULL for cmd_tbl"); return -1; }
        memset(tbl_virt, 0, tbl_size);
        p->cmd_tbl_phys[i] = tbl_phys;
        p->cmd_tbl_virt[i] = tbl_virt;
        HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER *)((uintptr_t)clb_virt + (i * sizeof(HBA_CMD_HEADER)));
        cmdheader->ctba = (uint32_t)(tbl_phys & 0xFFFFFFFF);
        cmdheader->ctbau = (uint32_t)((tbl_phys >> 32) & 0xFFFFFFFF);
        cmdheader->prdtl = 0;
    }

    port->is = (uint32_t)-1;
    port->ie = 0xFFFFFFFF;

    if (start_port(p) < 0) return -1;

    return 0;
}

static int port_device_present(HBA_PORT *port) {
    uint32_t ssts = port->ssts;
    uint32_t det = ssts & 0x0F;
    uint32_t ipm = (ssts >> 8) & 0x0F;
    if (det != 0x3) return 0;
    (void)ipm;
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
    cfis[7] = 0; 
    cfis[8] = (uint8_t)(count & 0xFF);
    cfis[9] = (uint8_t)((count >> 8) & 0xFF);
}

static int port_identify(ahci_port_t *p, uint64_t buffer_phys) {
    HBA_PORT *port = p->port;
    int slot = find_cmdslot(port);
    if (slot < 0) { AHCI_LOG("identify: no free slot"); return -1; }

    HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER *)((uintptr_t)p->clb_virt + (slot * sizeof(HBA_CMD_HEADER)));
    memset(cmdheader, 0, sizeof(HBA_CMD_HEADER));

    HBA_CMD_TBL *cmdtbl = (HBA_CMD_TBL *)p->cmd_tbl_virt[slot];
    memset(cmdtbl, 0, AHCI_CMD_TBL_SIZE);

    cmdheader->cfl = 5;
    cmdheader->w = 0;
    cmdheader->prdtl = 1;

    HBA_PRDT_ENTRY *prdt = &cmdtbl->prdt_entry[0];
    prdt->dba = (uint32_t)(buffer_phys & 0xFFFFFFFF);
    prdt->dbau = (uint32_t)((buffer_phys >> 32) & 0xFFFFFFFF);
    build_reg_h2d(cmdtbl->cfis, 0xEC, 0, 0, 0);
    port->ci = (1u << slot);

    unsigned int timeout = 5000; 
    unsigned int waited = 0;
    while (1) {
        if (!(port->ci & (1u << slot))) break;
        if (port->tfd & (0x88)) {
        }
        if (waited >= timeout) {
            AHCI_LOG("identify: timeout");
            return -1;
        }
        sleep_ms(1);
        waited++;
    }

    if (port->tfd & (1 << 0)) {
        AHCI_LOG("identify: device returned error in tfd");
        return -1;
    }

    return 0;
}

static int port_read_single(ahci_port_t *p, uint64_t buffer_phys, uint64_t start_lba) {
    HBA_PORT *port = p->port;
    int slot = find_cmdslot(port);
    if (slot < 0) { AHCI_LOG("read: no free slot"); return -1; }

    HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER *)((uintptr_t)p->clb_virt + (slot * sizeof(HBA_CMD_HEADER)));
    memset(cmdheader, 0, sizeof(HBA_CMD_HEADER));

    HBA_CMD_TBL *cmdtbl = (HBA_CMD_TBL *)p->cmd_tbl_virt[slot];
    memset(cmdtbl, 0, AHCI_CMD_TBL_SIZE);

    cmdheader->cfl = 5;
    cmdheader->w = 0; 
    cmdheader->prdtl = 1;

    HBA_PRDT_ENTRY *prdt = &cmdtbl->prdt_entry[0];
    prdt->dba = (uint32_t)(buffer_phys & 0xFFFFFFFF);
    prdt->dbau = (uint32_t)((buffer_phys >> 32) & 0xFFFFFFFF);
    prdt->dbc = 512 - 1;

    memset(cmdtbl->cfis, 0, 64);
    cmdtbl->cfis[0] = FIS_TYPE_REG_H2D;
    cmdtbl->cfis[1] = 1 << 7;
    cmdtbl->cfis[2] = 0x25;
    uint64_t lba = start_lba;
    cmdtbl->cfis[4] = (uint8_t)(lba & 0xFF);
    cmdtbl->cfis[5] = (uint8_t)((lba >> 8) & 0xFF);
    cmdtbl->cfis[6] = (uint8_t)((lba >> 16) & 0xFF);
    cmdtbl->cfis[7] = (uint8_t)((lba >> 24) & 0xFF);
    cmdtbl->cfis[8] = 1; 

    port->ci = (1u << slot);

    unsigned int timeout = 5000;
    unsigned int waited = 0;
    while (1) {
        if (!(port->ci & (1u << slot))) break;
        if (waited >= timeout) {
            AHCI_LOG("read: timeout");
            return -1;
        }
        sleep_ms(1);
        waited++;
    }

    if (port->tfd & (1 << 0)) {
        AHCI_LOG("read: device returned error in tfd");
        return -1;
    }

    return 0;
}

int ahci_init_from_pci(uint8_t bus, uint8_t slot, uint8_t func) {
    uint64_t bar0 = pci_get_bar64(bus, slot, func, 5); 
    if (!bar0) {
        AHCI_LOG("ahci_init_from_pci: no BAR found");
        return -1;
    }
    ahci_init_from_bar(bar0);
    return 0;
}

void utoa_dec(unsigned int val, char* buf, size_t bufsize) {
    if (bufsize == 0) return;

    buf[bufsize - 1] = '\0';
    int pos = bufsize - 2;

    if (val == 0) {
        buf[pos] = '0';
        memmove(buf, &buf[pos], 2); 
        return;
    }

    while (val && pos >= 0) {
        buf[pos--] = '0' + (val % 10);
        val /= 10;
    }

    memmove(buf, &buf[pos + 1], bufsize - pos - 1);
}

int ahci_init_from_bar(uint64_t bar_phys) {
    void *hba_virt = (void*)(uintptr_t)bar_phys;
    if (!hba_virt) {
        AHCI_LOG("ahci_init_from_bar: mmio_map failed");
        return -1;
    }
    HBA_MEM *hba = (HBA_MEM *)hba_virt;
    g_ahci.hba = hba;
    g_ahci.hba_phys = bar_phys;
    g_ahci.ports_implemented = hba->pi;
    hba->ghc |= HBA_GHC_AE;

    int count = 0;
    for (int i = 0; i < AHCI_MAX_PORTS; i++) {
        if (hba->pi & (1u << i)) count++;
    }
    g_ahci.port_count = count;

    for (int i = 0; i < AHCI_MAX_PORTS; i++) {
        if (!(hba->pi & (1u << i))) continue;
        ahci_port_t *p = &g_ports[i];
        p->port_no = i;
        p->port = hba_port(hba, i);

        char buf[64];
        AHCI_LOG("ahci: rebasing port");
        utoa_dec(i, buf, sizeof(buf));
        print(" port ", 0x07); print(buf, 0x07); print("\n", 0x07);

        if (port_rebase(&g_ahci, p) < 0) {
            AHCI_LOG("ahci: port_rebase failed");
            continue;
        }

        if (!port_device_present(p->port)) {
            AHCI_LOG("ahci: no device on port");
            continue;
        }

        AHCI_LOG("ahci: device present on port, identifying");
        uint64_t ident_phys = phys_alloc_contiguous(512, 512);
        void *ident_virt = (void*)(uintptr_t)ident_phys;
        if (!ident_phys || !ident_virt) {
            AHCI_LOG("ahci: failed to alloc identify buffer");
            continue;
        }
        memset(ident_virt, 0, 512);
        if (port_identify(p, ident_phys) == 0) {
            AHCI_LOG("ahci: identify successful");
        } else {
            AHCI_LOG("ahci: identify failed");
        }

        uint64_t read_phys = phys_alloc_contiguous(512, 512);
        void *read_virt = (void*)(uintptr_t)read_phys;
        if (read_phys && read_virt) {
            memset(read_virt, 0, 512);
            if (port_read_single(p, read_phys, 0) == 0) {
                AHCI_LOG("ahci: read LBA0 successful");
            } else {
                AHCI_LOG("ahci: read LBA0 failed");
            }
        } else {
            AHCI_LOG("ahci: failed to alloc read buffer");
        }
    }

    AHCI_LOG("ahci: init complete");
    return 0;
}

pci_device_t find_ahci_controller() {
    for (int bus = 0; bus < 256; bus++) {
        for (uint8_t slot = 0; slot < 32; slot++) {
            for (uint8_t func = 0; func < 8; func++) {
                uint16_t vendor = pci_config_read16(bus, slot, func, 0x00);
                if (vendor == 0xFFFF) continue;

                uint8_t class = pci_read_config8(bus, slot, func, 0x0B);
                uint8_t subclass = pci_read_config8(bus, slot, func, 0x0A);
                uint8_t prog_if = pci_read_config8(bus, slot, func, 0x09);
                uint8_t header_type = pci_read_config8(bus, slot, func, 0x0E);
                uint16_t device = pci_config_read16(bus, slot, func, 0x02);

                if (class == 0x01 && subclass == 0x06) { 
                    pci_device_t dev;
                    dev.bus = bus;
                    dev.slot = slot;
                    dev.function = func;
                    dev.vendor_id = vendor;
                    dev.device_id = device;
                    dev.class_id = class;
                    dev.subclass_id = subclass;
                    dev.prog_if = prog_if;
                    dev.header_type = header_type;

                    for (int i = 0; i < 6; i++)
                        dev.bar[i] = pci_config_read32(bus, slot, func, 0x10 + i*4);

                    return dev;
                }
            }
        }
    }

    pci_device_t empty = { .bus = 0xFF, .slot = 0xFF, .function = 0xFF };
    return empty;
}

void ahci_init() {
    pci_device_t ahci_dev = find_ahci_controller();
    if (ahci_dev.bus != 0xFF) {
        ahci_init_from_pci(ahci_dev.bus, ahci_dev.slot, ahci_dev.function);
    } else {
        print_warn("No AHCI controller found!");
    }
}