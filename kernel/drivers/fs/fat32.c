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
#include <fs/disk.h>
#include <types.h>
#include <string.h>
#include <memory.h>

static uint32_t fat32_cluster_to_lba(fat32_t *fs, uint32_t cluster) {
    return fs->data_start + (cluster - 2) * fs->sectors_per_cluster;
}

static uint32_t fat32_get_next_cluster(fat32_t *fs, uint32_t cluster) {
    uint32_t fat_offset = cluster * 4;
    uint32_t fat_sector = fs->fat_start + (fat_offset / fs->bpb.bytes_per_sector);
    uint32_t ent_offset = fat_offset % fs->bpb.bytes_per_sector;

    uint8_t sector[512];
    if (disk_read_sector(fs->disk, fat_sector, sector)) return 0x0FFFFFFF;
    return (*(uint32_t*)(sector + ent_offset)) & 0x0FFFFFFF;
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

int fat32_init(fat32_t *fs, disk_t disk, uint8_t partition) {
    uint8_t mbr[512];
    if (disk_read_sector(disk, 0, mbr)) return -1;

    if (*(uint16_t*)(mbr + 0x1FE) != 0xAA55) return -1;

    if (partition >= 4) return -1;
    uint8_t *entry = mbr + 0x1BE + partition * 16;
    if (entry[4] != 0x0B && entry[4] != 0x0C) return -1;

    fs->partition_start = *(uint32_t*)(entry + 8);
    fs->disk = disk;

    if (disk_read_sector(fs->disk, fs->partition_start, (uint8_t*)&fs->bpb)) return -1;

    if (*(uint16_t*)((uint8_t*)&fs->bpb + 0x1FE) != 0xAA55) return -1;
    if (strncmp(fs->bpb.fs_type, "FAT32   ", 8) != 0) return -1;

    fs->fat_start = fs->partition_start + fs->bpb.reserved_sectors;
    fs->data_start = fs->fat_start + (fs->bpb.num_fats * fs->bpb.sectors_per_fat);
    fs->sectors_per_cluster = fs->bpb.sectors_per_cluster;

    return 0;
}

int fat32_read_file(fat32_t *fs, const string name, uint8_t *buffer, uint32_t size) {
    char target_name[11];
    fat32_to_83_name(name, target_name);
    
    uint32_t current_cluster = fs->bpb.root_cluster;
    uint8_t sector[512];
    fat32_dir_entry_t *entry;

    do {
        uint32_t lba = fat32_cluster_to_lba(fs, current_cluster);
        for (uint8_t i = 0; i < fs->sectors_per_cluster; i++) {
            if (disk_read_sector(fs->disk, lba + i, sector)) return -1;
            
            for (int j = 0; j < 16; j++) {
                entry = (fat32_dir_entry_t*)(sector + j*32);
                
                if (entry->name[0] == 0x00) return -1;
                if (entry->attributes == 0x0F) continue; // Pomijaj wpisy długich nazw
                if (memcmp(entry->name, target_name, 11)) continue;

                uint32_t file_cluster = (entry->cluster_high << 16) | entry->cluster_low;
                uint32_t bytes_read = 0;
                
                while (file_cluster < 0x0FFFFFF8 && bytes_read < entry->file_size) {
                    lba = fat32_cluster_to_lba(fs, file_cluster);
                    for (int k = 0; k < fs->sectors_per_cluster; k++) {
                        if (disk_read_sector(fs->disk, lba + k, buffer + bytes_read)) return -1;
                        bytes_read += 512;
                        if (bytes_read >= entry->file_size) break;
                    }
                    file_cluster = fat32_get_next_cluster(fs, file_cluster);
                }
                return bytes_read > size ? size : bytes_read;
            }
        }
        current_cluster = fat32_get_next_cluster(fs, current_cluster);
    } while (current_cluster < 0x0FFFFFF8);

    return -1;
}