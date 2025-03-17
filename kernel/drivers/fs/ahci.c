/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include "ahci.h"
#include <pci.h>
#include <memory.h>
#include <types.h>

static HBA_MEM *abars;
static disk_t ahci_disks[32];
static uint8_t num_ahci_disks = 0;

static void port_rebase(HBA_PORT *port, int portnum) {
    port->clb = (uint32_t)malloc(1024) & 0xFFFFFFFF;
    port->clbu = 0;
    port->fb = (uint32_t)malloc(256) & 0xFFFFFFFF;
    port->fbu = 0;
    
    HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)port->clb;
    for(int i=0; i<32; i++) {
        cmdheader[i].prdtl = 8;
        cmdheader[i].ctba = (uint32_t)malloc(256) & 0xFFFFFFFF;
        cmdheader[i].ctbau = 0;
    }
}

void ahci_init() {
    pci_dev_t dev = pci_find_class(0x1, 0x6);
    if (dev.vendor_id == 0xFFFF) return;
    
    abars = (HBA_MEM*)(pci_read(dev, 0x24) & 0xFFFFFFF0);
    abars->ghc |= 0x80000000;
    
    uint32_t pi = abars->pi;
    for(int i=0; i<32; i++) {
        if(pi & (1 << i)) {
            HBA_PORT *port = &abars->ports[i];
            if((port->ssts & 0xF) != 3) continue; 
            
            port_rebase(port, i);
            ahci_disks[num_ahci_disks].type = DISK_TYPE_AHCI;
            ahci_disks[num_ahci_disks].ahci.ahci_base = (uint32_t)abars;
            ahci_disks[num_ahci_disks].ahci.port = i;
            ahci_disks[num_ahci_disks].is_atapi = false; 
            num_ahci_disks++;
        }
    }
}

disk_t* ahci_get_disks(uint8_t *count) {
    *count = num_ahci_disks;
    return ahci_disks;
}