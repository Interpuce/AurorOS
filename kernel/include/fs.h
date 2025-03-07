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

extern int fat32_read_file(const string name, uint8_t *buffer, uint32_t size);

string disk_read_file(string path) {
    uint32_t max = 0xFFFFFFFF;
    string buffer;
    fat32_read_file(path, buffer, max);
    return buffer;
}

bool disk_write_file(string path, string what) {
    return false;
}