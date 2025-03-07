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
#include <memory.h>

#define MEMORY_POOL_SIZE 1024 * 1024

typedef struct memory_block {
    size_t size;
    struct memory_block *next;
    int free;
} memory_block_t;

static uint8_t memory_pool[MEMORY_POOL_SIZE];
static memory_block_t *free_list = (memory_block_t *)memory_pool;

void init_memory() {
    free_list->size = MEMORY_POOL_SIZE - sizeof(memory_block_t);
    free_list->next = NULL;
    free_list->free = 1;
}

void *memcpy(void *dest, const void *src, size_t n) {
    unsigned char* d = dest;
    const unsigned char* s = src;
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

void *malloc(size_t size) {
    memory_block_t *current = free_list;
    while (current != NULL) {
        if (current->free && current->size >= size) {
            if (current->size > size + sizeof(memory_block_t)) {
                memory_block_t *new_block = (void *)((uint8_t *)current + sizeof(memory_block_t) + size);
                new_block->size = current->size - size - sizeof(memory_block_t);
                new_block->next = current->next;
                new_block->free = 1;

                current->size = size;
                current->next = new_block;
            }
            current->free = 0;
            return (void *)((uint8_t *)current + sizeof(memory_block_t));
        }
        current = current->next;
    }
    return NULL;
}

void free(void *ptr) {
    if (ptr == NULL) return;

    memory_block_t *block = (memory_block_t *)((uint8_t *)ptr - sizeof(memory_block_t));
    block->free = 1;

    if (block->next != NULL && block->next->free) {
        block->size += sizeof(memory_block_t) + block->next->size;
        block->next = block->next->next;
    }

    memory_block_t *current = free_list;
    while (current != NULL) {
        if (current->next == block && current->free) {
            current->size += sizeof(memory_block_t) + block->size;
            current->next = block->next;
            break;
        }
        current = current->next;
    }
}
