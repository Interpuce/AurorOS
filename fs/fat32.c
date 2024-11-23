/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include "fat32.h"
#include <system.h>
#include <types.h>

#define DISK_PORT_CONTROL 0x1F0 
#define DISK_PORT_ERROR 0x1F1 
#define DISK_PORT_FEATURES 0x1F1 
#define DISK_PORT_SECTOR_COUNT 0x1F2
#define DISK_PORT_LBA_LOW 0x1F3 
#define DISK_PORT_LBA_MID 0x1F4 
#define DISK_PORT_LBA_HIGH 0x1F5 
#define DISK_PORT_DEVICE 0x1F6
#define DISK_PORT_COMMAND 0x1F7

#define FAT32_SECTOR_SIZE 512
#define FAT32_MAX_CLUSTERS 0x0FFFFFF8

int fat32_lowlevel_read_sector(uint32_t lba, uint32_t count, uint8_t* buffer) {
    uint32_t sector;
    outb(DISK_PORT_DEVICE, 0xE0 | ((lba >> 24) & 0x0F));
    outb(DISK_PORT_SECTOR_COUNT, count);
    outb(DISK_PORT_LBA_LOW, lba & 0xFF);
    outb(DISK_PORT_LBA_MID, (lba >> 8) & 0xFF);
    outb(DISK_PORT_LBA_HIGH, (lba >> 16) & 0xFF);
    outb(DISK_PORT_COMMAND, 0x20);

    for (sector = 0; sector < count; sector++) {
        inw(DISK_PORT_CONTROL, buffer + sector * FAT32_SECTOR_SIZE, FAT32_SECTOR_SIZE / 2);
    }

    return 0;
}

int fat32_lowlevel_write_sector(uint32_t lba, uint32_t count, uint8_t* buffer) {
    uint32_t sector;
    outb(DISK_PORT_DEVICE, 0xE0 | ((lba >> 24) & 0x0F)); 
    outb(DISK_PORT_SECTOR_COUNT, count);
    outb(DISK_PORT_LBA_LOW, lba & 0xFF); 
    outb(DISK_PORT_LBA_MID, (lba >> 8) & 0xFF);
    outb(DISK_PORT_LBA_HIGH, (lba >> 16) & 0xFF);
    outb(DISK_PORT_COMMAND, 0x30);

    for (sector = 0; sector < count; sector++) {
        outw(DISK_PORT_CONTROL, *(uint16_t*)(buffer + sector * FAT32_SECTOR_SIZE));
    }

    return 0;
}

int fat32_read_sector(uint32_t lba, uint32_t count, uint8_t* buffer) {
    return fat32_lowlevel_read_sector(lba, count, buffer);
}

int fat32_get_boot_sector(fat32_boot_sector_t* boot_sector) {
    uint8_t buffer[FAT32_SECTOR_SIZE];
    if (fat32_read_sector(0, 1, buffer) != 0) {
        return -1;
    }
    memcpy(boot_sector, buffer, sizeof(fat32_boot_sector_t));
    return 0;
}

int fat32_get_fs_info(fat32_fs_info_t* fs_info) {
    uint8_t buffer[FAT32_SECTOR_SIZE];
    if (fat32_read_sector(FAT32_FS_INFO_SECTOR_OFFSET, 1, buffer) != 0) {
        return -1;
    }
    memcpy(fs_info, buffer, sizeof(fat32_fs_info_t));
    return 0;
}

int fat32_read_file(uint32_t cluster, uint8_t* buffer, uint32_t size) {
    uint32_t lba = cluster_to_lba(cluster);
    uint32_t sectors = (size + FAT32_SECTOR_SIZE - 1) / FAT32_SECTOR_SIZE;
    return fat32_read_sector(lba, sectors, buffer);
}

int fat32_write_file(uint32_t cluster, uint8_t* buffer, uint32_t size) {
    uint32_t lba = cluster_to_lba(cluster);
    uint32_t sectors = (size + FAT32_SECTOR_SIZE - 1) / FAT32_SECTOR_SIZE;
    return fat32_lowlevel_write_sector(lba, sectors, buffer);
}

uint32_t cluster_to_lba(uint32_t cluster) {
    fat32_boot_sector_t boot_sector;
    if (fat32_get_boot_sector(&boot_sector) != 0) {
        return 0;
    }
    uint32_t first_data_sector = boot_sector.reserved_sector_count + (boot_sector.number_of_fats * boot_sector.fat_size);
    return first_data_sector + ((cluster - 2) * boot_sector.sectors_per_cluster);
}