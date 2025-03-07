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

#include "fat32.h"
#include <ports.h>
#include <types.h>
#include <string.h>
#include <memory.h>

static struct {
    fat32_bpb_t bpb;
    uint32_t partition_start;
    uint32_t fat_start;
    uint32_t data_start;
    uint8_t sectors_per_cluster;
} fat32;

static void disk_read_sector(uint32_t lba, uint8_t *buffer) {
    outb(0x1F6, 0xE0 | ((lba >> 24) & 0x0F));
    outb(0x1F2, 1);
    outb(0x1F3, lba & 0xFF);
    outb(0x1F4, (lba >> 8) & 0xFF);
    outb(0x1F5, (lba >> 16) & 0xFF);
    outb(0x1F7, 0x20);

    while (inb(0x1F7) & 0x80) continue;

    for (uint16_t i = 0; i < 256; i++) {
        *((uint16_t*)buffer) = inw(0x1F0);
        buffer += 2;
    }
}

static uint32_t fat32_cluster_to_lba(uint32_t cluster) {
    return fat32.data_start + (cluster - 2) * fat32.sectors_per_cluster;
}

static uint32_t fat32_get_next_cluster(uint32_t cluster) {
    uint32_t fat_offset = cluster * 4;
    uint32_t fat_sector = fat32.fat_start + (fat_offset / fat32.bpb.bytes_per_sector);
    uint32_t ent_offset = fat_offset % fat32.bpb.bytes_per_sector;

    uint8_t sector[512];
    disk_read_sector(fat_sector, sector);
    return (*(uint32_t*)(sector + ent_offset)) & 0x0FFFFFFF;
}

int fat32_init() {
    uint8_t mbr[512];
    disk_read_sector(0, mbr);

    if (*(uint16_t*)(mbr + 0x1FE) != 0xAA55) return -1;

    for (int i = 0; i < 4; i++) {
        uint8_t *entry = mbr + 0x1BE + i*16;
        if (entry[4] == 0x0B || entry[4] == 0x0C) {
            fat32.partition_start = *(uint32_t*)(entry + 8);
            break;
        }
    }

    disk_read_sector(fat32.partition_start, (uint8_t*)&fat32.bpb);
    
    if (*(uint16_t*)((uint8_t*)&fat32.bpb + 0x1FE) != 0xAA55 ||
        strncmp(fat32.bpb.fs_type, "FAT32   ", 8) != 0) return -1;

    fat32.fat_start = fat32.partition_start + fat32.bpb.reserved_sectors;
    fat32.data_start = fat32.fat_start + (fat32.bpb.num_fats * fat32.bpb.sectors_per_fat);
    fat32.sectors_per_cluster = fat32.bpb.sectors_per_cluster;
    
    return 0;
}

static void fat32_to_83_name(const string name, string out) {
    memset(out, ' ', 11);
    const string dot = strchr(name, '.');
    
    int name_len = dot ? (dot - name) : strlen(name);
    if (name_len > 8) name_len = 8;
    for (int i = 0; i < name_len; i++) 
        out[i] = toupper(name[i]);

    if (dot) {
        int ext_len = strlen(dot + 1);
        if (ext_len > 3) ext_len = 3;
        for (int i = 0; i < ext_len; i++)
            out[8 + i] = toupper(dot[1 + i]);
    }
}

int fat32_read_file(const string name, uint8_t *buffer, uint32_t size) {
    char target_name[11];
    fat32_to_83_name(name, target_name);
    
    uint32_t current_cluster = fat32.bpb.root_cluster;
    uint8_t sector[512];
    fat32_dir_entry_t *entry;

    do {
        uint32_t lba = fat32_cluster_to_lba(current_cluster);
        for (uint8_t i = 0; i < fat32.sectors_per_cluster; i++) {
            disk_read_sector(lba + i, sector);
            
            for (int j = 0; j < 16; j++) {
                entry = (fat32_dir_entry_t*)(sector + j*32);
                
                if (entry->name[0] == 0x00) return -1; 
                // if (entry->name[0] == 0xE5) continue; 
                // ^ removed because impossible to become true
                if (memcmp(entry->name, target_name, 11)) continue;

                uint32_t file_cluster = (entry->cluster_high << 16) | entry->cluster_low;
                uint32_t bytes_read = 0;
                
                while (file_cluster < 0x0FFFFFF8 && bytes_read < entry->file_size) {
                    lba = fat32_cluster_to_lba(file_cluster);
                    for (int k = 0; k < fat32.sectors_per_cluster; k++) {
                        disk_read_sector(lba + k, buffer + bytes_read);
                        bytes_read += 512;
                        if (bytes_read >= entry->file_size) break;
                    }
                    file_cluster = fat32_get_next_cluster(file_cluster);
                }
                return bytes_read > size ? size : bytes_read;
            }
        }
        current_cluster = fat32_get_next_cluster(current_cluster);
    } while (current_cluster < 0x0FFFFFF8);

    return -1;
}