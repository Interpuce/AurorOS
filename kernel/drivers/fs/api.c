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

typedef enum {
    FS_UNKNOWN,
    FS_FAT32,
    FS_NTFS,
    FS_EXT2
} fs_type_t;

typedef struct {
    fs_type_t type;
    union {
        fat32_t fat32;
    };
    bool is_atapi;
} filesystem_t;

extern int fat32_read_file(fat32_t *fs, const string name, uint8_t *buffer, uint32_t size, bool is_atapi);
extern int fat32_init(fat32_t *fs, disk_t disk, uint8_t partition, bool is_atapi);
extern int disk_read_sector(disk_t *disk, uint32_t lba, uint8_t *buffer); // also in disk.h

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

int ahci_init(uint32_t *ahci_base) {
    pci_dev_t dev = pci_find_class(0x1, 0x6);
    if(dev.vendor_id == 0xFFFF) return -1;
    *ahci_base = pci_read(dev, 0x24) & 0xFFFFFFF0;
    HBA_MEM *hba = (HBA_MEM*)*ahci_base;
    hba->ghc |= 0x80000000;
    return 0;
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
    
    for (int i=0; i<4; i++) {
        fs_type_t type = detect_filesystem(ide_disks[i], 0);
        if(type != FS_UNKNOWN) {
            fs = init_filesystem(&ide_disks[i], type);
            break;
        }
    }
    
    for (int i=0; i<ahci_count; i++) {
        fs_type_t type = detect_filesystem(ahci_disks[i], 0);
        if(type != FS_UNKNOWN) {
            fs = init_filesystem(&ahci_disks[i], type);
            break;
        }
    }
}

FileReadResult disk_read_file(string path) {
    FileReadResult result = {0, "", false, false};
    if (!fs || fs->type == FS_UNKNOWN) return result;

    uint32_t max = 0xFFFFFFFF;
    switch(fs->type) {
        case FS_FAT32:
            result.bytes_read = fat32_read_file(&fs->fat32, path, (uint8_t*)result.content, max, fs->is_atapi);
            result.success = true;
            break;
        case FS_NTFS:
            print_error("Cannot read data from the unsupported filesystem!");
            break;
        case FS_EXT2:
            print_error("Cannot read data from the unsupported filesystem!");
            break;
    }
    return result;
}

bool disk_write_file(string path, string what) {
    if (!fs || fs->type == FS_UNKNOWN) return false;
    return false;
}