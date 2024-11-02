/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

// WARNING: THIS IS A BASIC DRIVER
// i think it contains some error because filesystems are complicated.

#include <types.h>
#include "fat32.h"

bool fat32_init(FAT32FileSystem *fs, uint8_t *disk_image) {
    memcpy(&fs->boot_sector, disk_image, SECTOR_SIZE);
    fs->first_data_sector = fs->boot_sector.reserved_sectors + (fs->boot_sector.num_fats * fs->boot_sector.fat_size_32);
    return true;
}

uint32_t fat32_get_next_cluster(FAT32FileSystem *fs, uint32_t cluster) {
    uint32_t fat_offset = cluster * 4; 
    uint32_t fat_sector = fs->boot_sector.reserved_sectors + (fat_offset / SECTOR_SIZE);
    uint32_t fat_entry_offset = fat_offset % SECTOR_SIZE;

    uint32_t next_cluster;
    memcpy(&next_cluster, &fs->disk_image[fat_sector * SECTOR_SIZE + fat_entry_offset], 4);
    return next_cluster & 0x0FFFFFFF;
}

void *internal_memcpy(void *dest, const void *src, size_t n) {
    unsigned char *d = dest;
    const unsigned char *s = src;
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

bool fat32_read_file(FAT32FileSystem *fs, const char *filename, uint8_t *buffer, uint32_t *size) {
    DirectoryEntry dir_entry;
    uint32_t cluster = fs->boot_sector.root_cluster;

    while (true) {
        uint32_t dir_sector = fs->first_data_sector + (cluster - 2) * fs->boot_sector.sectors_per_cluster;
        for (uint32_t i = 0; i < fs->boot_sector.sectors_per_cluster; i++) {
            memcpy(&dir_entry, &fs->disk_image[dir_sector * SECTOR_SIZE + i * sizeof(DirectoryEntry)], sizeof(DirectoryEntry));
            if (dir_entry.name[0] == 0) break; 
            if (strncmp(dir_entry.name, filename, FAT32_MAX_FILENAME) == 0) {
                cluster = dir_entry.first_cluster_low;
                *size = dir_entry.size;
                break;
            }
        }

        if (dir_entry.name[0] != 0) break;

        cluster = fat32_get_next_cluster(fs, cluster);
        if (cluster >= 0x0FFFFFF8) break;
    }

    uint32_t bytes_read = 0;
    while (bytes_read < *size) {
        uint32_t data_sector = fs->first_data_sector + (cluster - 2) * fs->boot_sector.sectors_per_cluster;
        
        uint32_t bytes_remaining = *size - bytes_read;
        uint32_t bytes_to_read = (bytes_remaining < FAT32_CLUSTER_SIZE) ? bytes_remaining : FAT32_CLUSTER_SIZE;

        my_memcpy(&buffer[bytes_read], &fs->disk_image[data_sector * SECTOR_SIZE], bytes_to_read);
        bytes_read += bytes_to_read;
        cluster = fat32_get_next_cluster(fs, cluster);
        
        if (cluster >= 0x0FFFFFF8) break;
    }

    return bytes_read == *size;
}