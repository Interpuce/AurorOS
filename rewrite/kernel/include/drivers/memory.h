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

extern uint32_t allocate_page();
extern void map_page(uint32_t phys_addr, uint32_t virt_addr);
extern void page_fault_handler();
extern void* memset(void* dest, int value, uint32_t size);
extern void* memcpy(void* dest, const void* src, uint32_t size);
extern void* memmove(void* dest, const void* src, uint32_t size);
extern int memcmp(const void* ptr1, const void* ptr2, uint32_t size);

// kernel level:
extern void* malloc(uint32_t size);
extern void* calloc(uint32_t num, uint32_t size);
extern void free(void* ptr);
extern void* find_free_block(uint32_t size);

// user level:
extern void* user_malloc(uint32_t size);
extern void* user_calloc(uint32_t num, uint32_t size);
extern void user_free(void* ptr);
extern void* user_find_free_block(uint32_t size);

// random shit
extern void init_memory();