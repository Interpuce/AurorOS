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

extern void *memcpy(void *dest, const void *src, size_t n);
extern void *malloc(size_t size);
extern void init_memory();
extern void free(void *ptr);
extern void *memset(void *ptr, int value, size_t num);
extern int memcmp(const void *ptr1, const void *ptr2, size_t num);