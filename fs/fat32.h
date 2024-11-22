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

#define SECTOR_SIZE 512
#define FAT32_MAX_FILENAME 11
#define FAT32_CLUSTER_SIZE (SECTOR_SIZE * 8) // 8 is just typical and previous configuration had errors
#define MAX_BUFFER_SIZE (SECTOR_SIZE * 10)

typedef struct {
    uint8_t jump[3];
    char oem[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t num_fats;
    uint16_t root_entries;
    uint16_t total_sectors_16;
    uint8_t media;
    uint16_t fat_size_16;
    uint16_t sectors_per_track;
    uint16_t num_heads;
    uint32_t hidden_sectors;
    uint32_t total_sectors_32;
    uint32_t fat_size_32;
    uint16_t extended_flags;
    uint16_t version;
    uint32_t root_cluster;
    uint16_t fsinfo;
    uint16_t backup_boot_sector;
    uint8_t reserved[12];
    uint8_t drive_number;
    uint8_t reserved_2;
    uint8_t signature;
    uint32_t serial_number;
    char label[11];
    char filesystem_type[8];
    uint8_t reserved_3[450];
    uint8_t boot_code[512 - 3 - 8 - 2 - 2 - 1 - 2 - 4 - 4 - 2 - 2 - 1 - 11 - 8 - 450];
} __attribute__((packed)) BootSector;

typedef struct {
    uint8_t status;
    char name[FAT32_MAX_FILENAME];
    uint8_t attributes;
    uint16_t reserved;
    uint16_t time;
    uint16_t date;
    uint16_t first_cluster_high;
    uint32_t size;
    uint16_t first_cluster_low;
} __attribute__((packed)) DirectoryEntry;

typedef struct {
    BootSector boot_sector;
    uint8_t *disk_image;
    uint32_t first_data_sector;
} FAT32FileSystem;

#endif