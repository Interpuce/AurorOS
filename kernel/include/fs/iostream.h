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

typedef struct {
    uint32_t bytes_read;
    string content;
    bool continous;
    bool success;
} FileReadResult;

extern void init_fs();
extern FileReadResult disk_read_file(string path);
extern bool disk_write_file(string path, string what);