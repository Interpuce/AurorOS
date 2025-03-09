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

typedef struct {
    uint32_t bytes_read;
    string content;
} FileReadResult;

FileReadResult disk_read_file(string path) {
    FileReadResult result = {0, ""};
    uint32_t max = 0xFFFFFFFF;
    result.bytes_read = fat32_read_file(path, result.content, max);
    return result;
}

bool disk_write_file(string path, string what) {
    return false;
}