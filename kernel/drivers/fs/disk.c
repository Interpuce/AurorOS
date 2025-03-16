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

uint16_t ata_get_disk_base_port(disk_t disk, uint8_t *drive) {
    switch (disk) {
        case DISK_PRIMARY_MASTER:
            *drive = 0;
            return 0x1F0;
        case DISK_PRIMARY_SLAVE:
            *drive = 1;
            return 0x1F0;
        case DISK_SECONDARY_MASTER:
            *drive = 0;
            return 0x170;
        case DISK_SECONDARY_SLAVE:
            *drive = 1;
            return 0x170;
        default:
            return 0; 
    }
}

int sata_disk_read_sector(uint16_t base_port, uint8_t drive, uint32_t lba, uint8_t *buffer) {
    outb(base_port + 6, 0x40 | (drive << 4));
    
    outb(base_port + 2, 0);
    outb(base_port + 3, (lba >> 24) & 0xFF);
    outb(base_port + 4, 0); 
    outb(base_port + 5, 0);

    outb(base_port + 2, 1); 
    outb(base_port + 3, lba & 0xFF);
    outb(base_port + 4, (lba >> 8) & 0xFF);
    outb(base_port + 5, (lba >> 16) & 0xFF);

    outb(base_port + 7, 0x24); 

    while (inb(base_port + 7) & 0x80);

    if (inb(base_port + 7) & 0x01) return -1;

    while (!(inb(base_port + 7) & 0x08));

    for (uint16_t i = 0; i < 256; i++) {
        uint16_t data = inw(base_port);
        *buffer++ = (uint8_t)(data & 0xFF);
        *buffer++ = (uint8_t)(data >> 8);
    }

    return 0;
}

int atapi_disk_read_sector(uint16_t base_port, uint8_t drive, uint32_t lba, uint8_t *buffer) {
    outb(base_port + 6, 0x40 | (drive << 4) | ((lba >> 24) & 0x0F));
    outb(base_port + 7, 0xA0);

    while (inb(base_port + 7) & 0x80);
    while (!(inb(base_port + 7) & 0x08));

    uint8_t cmd_packet[12] = {
        0xA8, 0x00,
        (uint8_t)((lba >> 24) & 0xFF),
        (uint8_t)((lba >> 16) & 0xFF),
        (uint8_t)((lba >> 8) & 0xFF),
        (uint8_t)(lba & 0xFF),
        0x00, 0x00, 0x00, 0x01, 0x00, 0x00
    };

    for (int i = 0; i < 6; i++) {
        outw(base_port, *(uint16_t*)(&cmd_packet[i * 2]));
    }

    while (inb(base_port + 7) & 0x80);
    if (inb(base_port + 7) & 0x01) return -1;

    uint16_t byte_count = (inb(base_port + 4) << 8) | inb(base_port + 5);
    for (uint16_t i = 0; i < byte_count / 2; i++) {
        uint16_t data = inw(base_port);
        *buffer++ = (uint8_t)(data & 0xFF);
        *buffer++ = (uint8_t)(data >> 8);
    }

    return 0;
}

int ata_disk_read_sector(uint16_t base_port, uint8_t drive, uint32_t lba, uint8_t *buffer) {
    outb(base_port + 6, 0x40 | (drive << 4) | ((lba >> 24) & 0x0F));
    outb(base_port + 2, 1);
    outb(base_port + 3, (uint8_t)(lba & 0xFF));
    outb(base_port + 4, (uint8_t)((lba >> 8) & 0xFF));
    outb(base_port + 5, (uint8_t)((lba >> 16) & 0xFF));
    outb(base_port + 7, 0x20);

    while (inb(base_port + 7) & 0x80);

    for (uint16_t i = 0; i < 256; i++) {
        uint16_t data = inw(base_port);
        *buffer++ = (uint8_t)(data & 0xFF);
        *buffer++ = (uint8_t)(data >> 8);
    }

    return 0;
}

int disk_read_sector(disk_t disk, uint32_t lba, uint8_t *buffer, bool is_atapi, bool is_sata) {
    uint8_t drive;
    uint16_t base_port = ata_get_disk_base_port(disk, &drive);

    if (base_port == 0) {
        return -1; 
    }

    if (is_sata) {
        return sata_disk_read_sector(base_port, drive, lba, buffer); // why the fuck sata uses ata ports ToDo i don't have time today
    } else if (is_atapi) {
        return atapi_disk_read_sector(base_port, drive, lba, buffer);
    } else {
        return ata_disk_read_sector(base_port, drive, lba, buffer);
    }
}