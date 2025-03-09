/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#pragma once

#include <types.h>
#include <fs/fat32.h>

extern int fat32_read_file(fat32_t *fs, const string name, uint8_t *buffer, uint32_t size);
extern int fat32_init(fat32_t *fs, disk_t disk, uint8_t partition);

fat32_t *fs;

void init_fs() {
    fat32_init(fs, DISK_PRIMARY_MASTER, 0);
}

typedef struct {
    uint32_t bytes_read;
    string content;
    bool continous;
} FileReadResult;

FileReadResult disk_read_file(string path) {
    FileReadResult result = {0, "", false};
    uint32_t max = 0xFFFFFFFF;
    result.bytes_read = fat32_read_file(fs, path, result.content, max);
    return result;
}

bool disk_write_file(string path, string what) {
    return false;
}