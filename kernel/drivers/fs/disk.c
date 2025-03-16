/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <fs/disk.h>
#include <ports.h>
#include "ahci.h"

int ide_read_sector(disk_t *disk, uint32_t lba, uint8_t *buffer) {
    uint16_t base = disk->ide.base_port;
    uint8_t drive = 0xA0 | (disk->ide.drive << 4) | ((lba >> 24) & 0x0F);
    
    outb(base + 6, drive);
    outb(base + 2, 1);
    outb(base + 3, (uint8_t)lba);
    outb(base + 4, (uint8_t)(lba >> 8));
    outb(base + 5, (uint8_t)(lba >> 16));
    outb(base + 7, 0x20);

    while((inb(base + 7) & 0x80));
    
    for(int i=0; i<256; i++) {
        uint16_t data = inw(base);
        *buffer++ = data & 0xFF;
        *buffer++ = data >> 8;
    }
    return 0;
}

int ahci_read_sector(disk_t *disk, uint32_t lba, uint8_t *buffer) {
    HBA_MEM *hba = (HBA_MEM*)disk->ahci.ahci_base;
    HBA_PORT *port = &hba->ports[disk->ahci.port];
    
    HBA_CMD_HEADER *cmdheader = (HBA_CMD_HEADER*)port->clb;
    cmdheader->cfl = sizeof(FIS_REG_H2D)/4;
    cmdheader->w = 0;
    cmdheader->prdtl = 1;
    
    HBA_CMD_TBL *cmdtbl = (HBA_CMD_TBL*)cmdheader->ctba;
    memset(cmdtbl, 0, sizeof(HBA_CMD_TBL));
    
    FIS_REG_H2D *fis = (FIS_REG_H2D*)cmdtbl->cfis;
    fis->type = FIS_TYPE_REG_H2D;
    fis->c = 1;
    fis->command = 0x25; // READ DMA EXT
    fis->lba0 = (uint8_t)lba;
    fis->lba1 = (uint8_t)(lba >> 8);
    fis->lba2 = (uint8_t)(lba >> 16);
    fis->device = 1 << 6; // LBA mode
    fis->lba3 = (uint8_t)(lba >> 24);
    fis->lba4 = (uint8_t)(lba >> 32);
    fis->lba5 = (uint8_t)(lba >> 40);
    fis->countl = 1;
    
    cmdtbl->prdt_entry[0].dba = (uint32_t)buffer & 0xFFFFFFFF;
    cmdtbl->prdt_entry[0].dbau = 0;
    cmdtbl->prdt_entry[0].dbc = 511;
    cmdtbl->prdt_entry[0].i = 1;
    
    port->ci = 1;
    while(port->ci & 1);
    
    return (port->is & 1) ? -1 : 0;
}

int disk_read_sector(disk_t *disk, uint32_t lba, uint8_t *buffer) {
    if(disk->type == DISK_TYPE_IDE)
        return ide_read_sector(disk, lba, buffer);
    else if(disk->type == DISK_TYPE_AHCI)
        return ahci_read_sector(disk, lba, buffer);
    return -1;
}