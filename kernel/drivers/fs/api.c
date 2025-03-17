/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <types.h>
#include <fs/fat32.h>
#include <fs/disk.h>
#include <fs/iostream.h>
#include <string.h>
#include <msg.h>
#include <memory.h>
#include <ports.h>
#include <pci.h>
#include "ahci.h"
#include "multiapi.h"

filesystem_t *fs;

bool detect_atapi(disk_t disk) {
    uint16_t base_port = disk.ide.base_port;
    uint8_t drive = disk.ide.drive;

    outb(base_port + 6, 0xA0 | (drive << 4));
    outb(base_port + 7, 0xA1);

    while (inb(base_port + 7) & 0x80);
    if (inb(base_port + 7) & 0x01) return false;

    return (inb(base_port + 4) == 0x14 && inb(base_port + 5) == 0xEB);
}

fs_type_t detect_filesystem(disk_t disk, uint8_t partition) {
    uint8_t mbr[512];
    
    if (!disk_read_sector(&disk, 0, mbr)) return FS_UNKNOWN;

    uint8_t *partition_entry = mbr + 0x1BE + (partition * 16);
    uint8_t type = partition_entry[4];
    uint32_t lba_start = *(uint32_t*)(partition_entry + 8);

    uint8_t boot_sector[512];
    if (!disk_read_sector(&disk, lba_start, boot_sector)) return FS_UNKNOWN;

    if (type == 0x0B || type == 0x0C) {
        if (strncmp((char*)boot_sector + 0x52, "FAT32   ", 8) == 0) {
            return FS_FAT32;
        }
    }

    if (type == 0x07) {
        if (strncmp((char*)boot_sector + 0x03, "NTFS", 4) == 0) {
            return FS_NTFS;
        }
    }

    if (type == 0x83) {
        uint8_t superblock[512];
        if (disk_read_sector(&disk, lba_start + 2, superblock)) {
            uint16_t magic = *(uint16_t*)(superblock + 0x38);
            if (magic == 0xEF53) return FS_EXT2;
        }
    }

    return FS_UNKNOWN;
}

void init_fs() {
    disk_t ide_disks[] = {
        {DISK_TYPE_IDE, .ide={0x1F0, 0}, .is_atapi=detect_atapi((disk_t){DISK_TYPE_IDE, .ide={0x1F0, 0}})},
        {DISK_TYPE_IDE, .ide={0x1F0, 1}, .is_atapi=detect_atapi((disk_t){DISK_TYPE_IDE, .ide={0x1F0, 1}})},
        {DISK_TYPE_IDE, .ide={0x170, 0}, .is_atapi=detect_atapi((disk_t){DISK_TYPE_IDE, .ide={0x170, 0}})},
        {DISK_TYPE_IDE, .ide={0x170, 1}, .is_atapi=detect_atapi((disk_t){DISK_TYPE_IDE, .ide={0x170, 1}})},
    };
    
    uint32_t ahci_base;
    ahci_init(&ahci_base);
    uint8_t ahci_count;
    disk_t *ahci_disks = ahci_get_disks(&ahci_count);
    
    for (int i=0; i < 4; i++) {
        fs_type_t type = detect_filesystem(ide_disks[i], 0);

        switch (type) {
            case FS_FAT32:
                fat32_t fat32;
                fat32_init(&fat32, ide_disks[i], 0, ide_disks->is_atapi);
                fs->fat32 = fat32;
                break;
            case FS_NTFS:
            case FS_EXT2:
            case FS_UNKNOWN:
            default:
                print_error('Error during initializing IDE/IDE-emulated (configure SATA as IDE) disk - unsupported/unknown filesystem.');
                break;
        }
    }
    
    for (int i=0; i < ahci_count; i++) {
        fs_type_t type = detect_filesystem(ahci_disks[i], 0);

        switch (type) {
            case FS_FAT32:
                fat32_t fat32;
                fat32_init(&fat32, ahci_disks[i], 0, false); // sata does not support atapi i think
                break;
            case FS_NTFS:
            case FS_EXT2:
            case FS_UNKNOWN:
            default:
                print_error('Error during initializing AHCI disk - unsupported/unknown filesystem.');
                break;
        }
    }
}