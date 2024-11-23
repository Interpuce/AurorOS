/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#ifndef FS_FAT32
#define FS_FAT32 1

#include <types.h>

#define FAT32_SECTOR_SIZE 512
#define FAT32_BOOT_SECTOR_OFFSET 0x0
#define FAT32_FS_INFO_SECTOR_OFFSET 0x1E0

typedef struct {
    uint8_t jump[3];
    uint8_t oem_name[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sector_count;
    uint8_t number_of_fats;
    uint16_t root_entry_count;
    uint16_t total_sectors_short;
    uint8_t media_descriptor;
    uint16_t fat_size;
    uint16_t sectors_per_track;
    uint16_t head_count;
    uint32_t hidden_sectors;
    uint32_t total_sectors_long;
} fat32_boot_sector_t;

typedef struct {
    uint32_t total_sectors;
    uint32_t fat_size;
    uint32_t root_cluster;
} fat32_fs_info_t;

int fat32_read_sector(uint32_t lba, uint32_t count, uint8_t* buffer);
int fat32_get_boot_sector(fat32_boot_sector_t* boot_sector);
int fat32_get_fs_info(fat32_fs_info_t* fs_info);
int fat32_read_file(uint32_t cluster, uint8_t* buffer, uint32_t size);
int fat32_write_file(uint32_t cluster, uint8_t* buffer, uint32_t size);

#endif