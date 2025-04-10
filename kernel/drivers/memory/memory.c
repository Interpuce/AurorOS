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

uint32_t kernel_space_size;
uint32_t user_space_end;
uint32_t kernel_heap_start;
uint32_t kernel_heap_end;
uint32_t user_heap_end;

typedef struct {
    uint32_t start_address;
    uint32_t end_address;
    const char* name;
} ReservedMemoryRegion;

ReservedMemoryRegion reserved_memory[] = {
    { 0x000E0000, 0x000FFFFF, "ACPI Tables" },
    { 0xB8000, 0xBFFFF, "VGA Memory" },
    { 0xF0000, 0xFFFFF, "BIOS ROM" }
};

void init_memory() {
    uint32_t total_memory = mttr_get_total_memory();
    kernel_space_size = (total_memory > MAX_KERNEL_MEMORY) ? MAX_KERNEL_MEMORY : total_memory;
    user_space_end = total_memory - kernel_space_size;

    kernel_heap_start = user_space_end;
    kernel_heap_end = kernel_heap_start + kernel_space_size;
    user_heap_end = user_space_end - 1;
}

void page_fault_handler() {
    do_kernel_panic("PAGE_FAULT", NULL);
}

uint32_t allocate_page() {
    uint32_t page = next_free_page;
    next_free_page += PAGE_SIZE;
    return page;
}

void map_page(uint32_t phys_addr, uint32_t virt_addr) {
    if (is_reserved_memory(phys_addr) || is_reserved_memory(virt_addr)) {
        do_kernel_panic("RESERVED_MEMORY_ACCESS", NULL);
    }

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

    if (heap_current + sizeof(Block) + size > kernel_heap_end || heap_current + sizeof(Block) + size > user_space_end) {
        do_kernel_panic("OUT_OF_MEMORY", NULL);
        return NULL;
    }

    while (is_reserved_memory(heap_current)) {
        heap_current += PAGE_SIZE; 
        if (heap_current + sizeof(Block) + size > kernel_heap_end || heap_current + sizeof(Block) + size > user_space_end) {
            do_kernel_panic("OUT_OF_MEMORY", NULL);
            return NULL;
        }
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

// user mode: 

void* user_malloc(uint32_t size) {
    size = (size + 3) & ~3; 

    if (heap_current + sizeof(Block) + size > user_heap_end) {
        do_kernel_panic("OUT_OF_MEMORY", NULL);
        return NULL;
    }

    Block* new_block = (Block*) heap_current;
    new_block->size = size;
    heap_current += sizeof(Block) + size;

    return (void*)(new_block + 1);
}

void* user_calloc(uint32_t num, uint32_t size) {
    uint32_t total = num * size;
    void* ptr = user_malloc(total);
    if (!ptr) return NULL;

    return memset(ptr, 0, total);
}

void user_free(void* ptr) {
    if (!ptr) return;

    Block* block = (Block *)ptr - 1;
    block->next = free_list;
    free_list = block;
}

void* user_find_free_block(uint32_t size) {
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
