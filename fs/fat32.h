/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <types.h>

#define SECTOR_SIZE 512
#define FAT32_CLUSTER_SIZE 4096 // the most mysterious define; some sources think that it can be modified to preferences but if there is any contributor who understand this - add a comment
#define MAX_ROOT_DIR_ENTRIES 512

typedef struct {
    uint8_t  jump[3];
    uint8_t  oem[8];
    uint16_t bytes_per_sector;
    uint8_t  sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t  num_fats;
    uint16_t max_root_dir_entries;
    uint16_t total_sectors_16;
    uint8_t  media_descriptor;
    uint16_t fat_size_16;
    uint16_t sectors_per_track;
    uint16_t heads;
    uint32_t hidden_sectors;
    uint32_t total_sectors_32;
    uint32_t fat_size_32;
    uint16_t ext_flags;
    uint16_t version;
    uint32_t root_cluster;
    uint16_t fsinfo_sector;
    uint16_t backup_boot_sector;
    uint8_t  reserved[12];
    uint8_t  drive_number;
    uint8_t  reserved1;
    uint8_t  boot_signature;
    uint32_t volume_id;
    uint8_t  volume_label[11];
    uint8_t  filesystem_type[8];
} __attribute__((packed)) fat32_boot_sector_t;

typedef struct {
    uint8_t  filename[11];
    uint8_t  attributes;
    uint8_t  reserved[10];
    uint16_t time_created;
    uint16_t date_created;
    uint16_t last_access_date;
    uint16_t first_cluster_high;
    uint16_t time_modified;
    uint16_t date_modified;
    uint16_t first_cluster_low;
    uint32_t file_size;
} __attribute__((packed)) fat32_directory_entry_t;
