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

#define PAGE_SIZE 0x1000
#define PAGE_DIR_INDEX(addr) (((uint32_t)(addr)) >> 22)
#define PAGE_TABLE_INDEX(addr) (((uint32_t)(addr) >> 12) & 0x3FF)

uint32_t page_tables[1024][1024];

uint32_t allocate_page() {
    static uint32_t next_free_page = 0x100000;
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

void enable_segmentation() {
    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r" (cr0));
    cr0 |= 0x1; 
    asm volatile("mov %0, %%cr0" : : "r" (cr0));
}