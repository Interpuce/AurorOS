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

#define PAGE_SIZE 0x1000
#define PAGE_DIR_INDEX(addr) (((uint32_t)(addr)) >> 22)
#define PAGE_TABLE_INDEX(addr) (((uint32_t)(addr) >> 12) & 0x3FF)

#define HEAP_START 0x100000
#define HEAP_END   0x2000000

typedef struct Block {
    uint32_t size;
    struct Block *next;
} Block;