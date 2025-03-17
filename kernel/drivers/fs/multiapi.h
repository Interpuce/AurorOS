/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <fs/fat32.h>

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
extern int disk_read_sector(disk_t *disk, uint32_t lba, uint8_t *buffer);

extern filesystem_t *fs;