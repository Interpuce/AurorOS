/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include "disk.h"
#include <ports.h>

int disk_read_sector(disk_t disk, uint32_t lba, uint8_t *buffer) {
    uint16_t base_port;
    uint8_t drive;

    switch (disk) {
        case DISK_PRIMARY_MASTER:
            base_port = 0x1F0;
            drive = 0;
            break;
        case DISK_PRIMARY_SLAVE:
            base_port = 0x1F0;
            drive = 1;
            break;
        case DISK_SECONDARY_MASTER:
            base_port = 0x170;
            drive = 0;
            break;
        case DISK_SECONDARY_SLAVE:
            base_port = 0x170;
            drive = 1;
            break;
        default:
            return -1;
    }

    outb(base_port + 6, 0x40 | (drive << 4) | ((lba >> 24) & 0x0F));
    outb(base_port + 2, 1);
    outb(base_port + 3, (uint8_t)(lba & 0xFF));
    outb(base_port + 4, (uint8_t)((lba >> 8) & 0xFF));
    outb(base_port + 5, (uint8_t)((lba >> 16) & 0xFF));
    outb(base_port + 7, 0x20);

    while (inb(base_port + 7) & 0x80) continue;

    for (uint16_t i = 0; i < 256; i++) {
        uint16_t data = inw(base_port);
        *buffer++ = (uint8_t)(data & 0xFF);
        *buffer++ = (uint8_t)(data >> 8);
    }

    return 0;
}