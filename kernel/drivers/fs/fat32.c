/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include "fat32.h"
#include <ports.h>
#include <msg.h>

static FAT32_BPB bpb;
static uint32_t fat_start, data_start;

void read_sector(uint32_t lba, void *buffer) {
    outb(0x1F6, (lba >> 24) | 0xE0);
    outb(0x1F2, 1);
    outb(0x1F3, lba & 0xFF);
    outb(0x1F4, (lba >> 8) & 0xFF);
    outb(0x1F5, (lba >> 16) & 0xFF);
    outb(0x1F7, 0x20);

    while (!(inb(0x1F7) & 0x08));

    for (int i = 0; i < 256; i++) {
        ((uint16_t*)buffer)[i] = inw(0x1F0);
    }
}

void fat32_init() {
    read_sector(0, &bpb);
    fat_start = bpb.reserved_sectors;
    data_start = fat_start + (bpb.fat_count * bpb.fat_size_32);
}

uint32_t fat32_next_cluster(uint32_t cluster) {
    uint32_t fat_sector = fat_start + (cluster * 4 / SECTOR_SIZE);
    uint32_t fat_offset = (cluster * 4) % SECTOR_SIZE;
    
    uint8_t buffer[SECTOR_SIZE];
    read_sector(fat_sector, buffer);
    
    return *(uint32_t*)&buffer[fat_offset] & 0x0FFFFFFF;
}

void fat32_list_directory(uint32_t cluster) {
    uint8_t buffer[SECTOR_SIZE];
    uint32_t sector = data_start + (cluster - 2) * bpb.sectors_per_cluster;

    read_sector(sector, buffer);

    FAT32_DirectoryEntry *entry = (FAT32_DirectoryEntry *)buffer;
    for (int i = 0; i < SECTOR_SIZE / sizeof(FAT32_DirectoryEntry); i++) {
        if (entry[i].name[0] == 0x00) break;
        if (entry[i].attr != 0x0F) {
            char name[12];
            for (int j = 0; j < 11; j++) name[j] = entry[i].name[j];
            name[11] = '\0';
            println(name, 0x07);
        }
    }
}

void fat32_read_file(const char *filename) {
    uint8_t buffer[SECTOR_SIZE];
    uint32_t cluster = bpb.root_cluster;

    while (cluster < 0x0FFFFFF8) {
        uint32_t sector = data_start + (cluster - 2) * bpb.sectors_per_cluster;
        read_sector(sector, buffer);

        FAT32_DirectoryEntry *entry = (FAT32_DirectoryEntry *)buffer;
        for (int i = 0; i < SECTOR_SIZE / sizeof(FAT32_DirectoryEntry); i++) {
            if (entry[i].name[0] == 0x00) return;
            if (entry[i].attr != 0x0F) {
                char name[12];
                for (int j = 0; j < 11; j++) name[j] = entry[i].name[j];
                name[11] = '\0';
                if (strcmp(name, filename) == 0) {
                    uint32_t file_cluster = ((uint32_t)entry[i].first_cluster_high << 16) | entry[i].first_cluster_low;
                    uint32_t size = entry[i].file_size;

                    while (size > 0) {
                        read_sector(data_start + (file_cluster - 2) * bpb.sectors_per_cluster, buffer);
                        for (int j = 0; j < SECTOR_SIZE && size > 0; j++, size--)
                            print(buffer[j], 0x07);
                        file_cluster = fat32_next_cluster(file_cluster);
                    }
                    return;
                }
            }
        }
        cluster = fat32_next_cluster(cluster);
    }
}