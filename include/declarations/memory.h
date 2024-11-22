/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#ifndef INCLUDE_DECLARATIONS_LANG
#define INCLUDE_DECLARATIONS_LANG 1

#include <types.h>

extern void *mem_alloc(size_t size);
extern void mem_free(void *ptr);
extern void memory_init(void);

extern void *memcpy(void *dest, const void *src, size_t n);

#endif