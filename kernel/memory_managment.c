/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <types.h>

#define MEMORY_POOL_SIZE  (1024 * 1024) // 1 MB memory pool
static uint8_t memory_pool[MEMORY_POOL_SIZE];
static uint32_t next_free_block = 0;

void *mem_alloc(size_t size) {
    if (next_free_block + size > MEMORY_POOL_SIZE) {
        return NULL; // Not enough memory
    }
    
    void *allocated_memory = &memory_pool[next_free_block];
    next_free_block += size;
    
    return allocated_memory;
}

void mem_free(void *ptr) {
    // Simple memory pool doesn't support freeing specific memory blocks.
    // In a more advanced memory manager, we would need to handle this.
    // For now, we will just reset the pool when we need to free.
    next_free_block = 0;
}

void memory_init() {
    next_free_block = 0; // Reset the memory pool pointer
}

void *memcpy(void *dest, const void *src, size_t n) {
    uint8_t *d = dest;
    const uint8_t *s = src;
    
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }

    return dest;
}