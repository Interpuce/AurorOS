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

void page_fault_handler() {
    do_kernel_panic("PAGE_FAULT", NULL);
}

void* malloc(uint32_t size) {
    if (heap_current + size > HEAP_END) {
        do_kernel_panic("OUT_OF_MEMORY", NULL);
        return NULL;
    }

    uint32_t allocated_memory = heap_current;
    heap_current += size;

    return (void*) allocated_memory;
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

void enable_segmentation() {
    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r" (cr0));
    cr0 |= 0x1; 
    asm volatile("mov %0, %%cr0" : : "r" (cr0));
}