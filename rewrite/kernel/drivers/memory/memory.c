/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <types.h>
#include <panic.h>
#include "mttr.h" // mttr_get_total_memory()
#include "memory.h"

uint32_t page_tables[1024][1024];
uint32_t next_free_page = 0x100000;
uint32_t heap_current = HEAP_START;
Block* free_list = NULL;

void page_fault_handler() {
    do_kernel_panic("PAGE_FAULT", NULL);
}

uint32_t allocate_page() {
    uint32_t page = next_free_page;
    next_free_page += PAGE_SIZE;
    return page;
}

void map_page(uint32_t phys_addr, uint32_t virt_addr) {
    uint32_t dir_index = PAGE_DIR_INDEX(virt_addr);
    uint32_t table_index = PAGE_TABLE_INDEX(virt_addr);
    page_tables[dir_index][table_index] = phys_addr | 0x03;
}

void* memset(void* dest, int value, uint32_t size) {
    uint8_t* ptr = (uint8_t*) dest;
    for (uint32_t i = 0; i < size; i++) {
        ptr[i] = (uint8_t) value;
    }
    return dest;
}

void* memcpy(void* dest, const void* src, uint32_t size) {
    uint8_t* d = (uint8_t*) dest;
    const uint8_t* s = (const uint8_t*) src;
    for (uint32_t i = 0; i < size; i++) {
        d[i] = s[i];
    }
    return dest;
}

void* memmove(void* dest, const void* src, uint32_t size) {
    uint8_t* d = (uint8_t*) dest;
    const uint8_t* s = (const uint8_t*) src;

    if (d < s) {
        for (uint32_t i = 0; i < size; i++) {
            d[i] = s[i];
        }
    } else if (d > s) {
        for (int i = size - 1; i >= 0; i--) {
            d[i] = s[i];
        }
    }
    return dest;
}

int memcmp(const void* ptr1, const void* ptr2, uint32_t size) {
    const uint8_t* p1 = (const uint8_t*) ptr1;
    const uint8_t* p2 = (const uint8_t*) ptr2;

    for (uint32_t i = 0; i < size; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] - p2[i];
        }
    }
    return 0;
}

void* malloc(uint32_t size) {
    size = (size + 3) & ~3;

    void* block = find_free_block(size);
    if (block) return block;

    if (heap_current + sizeof(Block) + size > HEAP_END) {
        do_kernel_panic("OUT_OF_MEMORY", NULL);
        return NULL;
    }

    Block* new_block = (Block*) heap_current;
    new_block->size = size;
    heap_current += sizeof(Block) + size;

    return (void*)(new_block + 1);
}

void* calloc(uint32_t num, uint32_t size) {
    uint32_t total = num * size;
    void* ptr = malloc(total);
    if (!ptr) return NULL;

    return memset(ptr, 0, total);
}

void free(void* ptr) {
    if (!ptr) return;

    Block* block = (Block *)ptr - 1;
    block->next = free_list;
    free_list = block;
}

void* find_free_block(uint32_t size) {
    Block* prev = NULL;
    Block* curr = free_list;

    while (curr) {
        if (curr->size >= size) {
            if (prev) {
                prev->next = curr->next;
            } else {
                free_list = curr->next;
            }
            return (void*) (curr + 1);
        }
        prev = curr;
        curr = curr->next;
    }
    return NULL;
}