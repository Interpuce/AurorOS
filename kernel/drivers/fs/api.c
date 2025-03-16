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
#include <fs/iostream.h>
#include <string.h>
#include <msg.h>
#include <memory.h>
#include <ports.h>

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
extern int disk_read_sector(disk_t disk, uint32_t lba, uint8_t *buffer, bool is_atapi, bool is_sata); // also in disk.h

filesystem_t *fs;

bool detect_atapi(disk_t disk) {
    uint16_t base_port;
    uint8_t drive;

    switch (disk) {
        case DISK_PRIMARY_MASTER: base_port=0x1F0; drive=0; break;
        case DISK_PRIMARY_SLAVE:  base_port=0x1F0; drive=1; break;
        case DISK_SECONDARY_MASTER: base_port=0x170; drive=0; break;
        case DISK_SECONDARY_SLAVE: base_port=0x170; drive=1; break;
        default: return false;
    }

    outb(base_port + 6, 0xA0 | (drive << 4));
    outb(base_port + 7, 0xA1);

    while (inb(base_port + 7) & 0x80);
    if (inb(base_port + 7) & 0x01) return false;

    return (inb(base_port + 4) == 0x14 && inb(base_port + 5) == 0xEB);
}

fs_type_t detect_filesystem(disk_t disk, uint8_t partition, bool is_atapi) {
    uint8_t mbr[512];
    
    if (!disk_read_sector(disk, 0, mbr, is_atapi, false)) return FS_UNKNOWN;

    uint8_t *partition_entry = mbr + 0x1BE + (partition * 16);
    uint8_t type = partition_entry[4];
    uint32_t lba_start = *(uint32_t*)(partition_entry + 8);

    uint8_t boot_sector[512];
    if (!disk_read_sector(disk, lba_start, boot_sector, is_atapi, false)) return FS_UNKNOWN;

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
        if (disk_read_sector(disk, lba_start + 2, superblock, is_atapi, false)) {
            uint16_t magic = *(uint16_t*)(superblock + 0x38);
            if (magic == 0xEF53) return FS_EXT2;
        }
    }

    return FS_UNKNOWN;
}

void init_fs() {
    fs = malloc(sizeof(filesystem_t));
    fs->is_atapi = detect_atapi(DISK_PRIMARY_MASTER);
    fs->type = detect_filesystem(DISK_PRIMARY_MASTER, 0, fs->is_atapi);
    
    switch(fs->type) {
        case FS_FAT32:
            fat32_init(&fs->fat32, DISK_PRIMARY_MASTER, 0, fs->is_atapi);
            print_ok("Successfully initialized FAT32 partition!");
            break;
        case FS_NTFS:
            print_info("Unfortunately the NTFS filesystem is not supported.");
            print_info("Skipping the partition...");
            break;
        case FS_EXT2:
            print_info("Unfortunately the EXT2 filesystem is not supported.");
            print_info("Skipping the partition...");
            break;
        default:
            fs->type = FS_UNKNOWN;
            break;
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