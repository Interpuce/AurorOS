#pragma once
#include <types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PM_PAGE_SIZE 4096UL

void init_memory_heap();
void *malloc(size_t size);
void free(void *ptr);
void *malloc_aligned(size_t size, size_t alignment);

void *memcpy(void *dest, const void *src, size_t n);
void *memmove(void *dest, const void *src, size_t n);
void *memset(void *ptr, int value, size_t num);

void physmem_init_from_multiboot(uint32_t mmap_addr, uint32_t mmap_length);
uint64_t phys_alloc_contiguous(size_t size, size_t alignment);
void phys_free_contiguous(uint64_t phys_addr, size_t size);

void mark_page_used(uint64_t page_index);
void mark_page_free(uint64_t page_index);
int page_is_free(uint64_t page_index);

uint64_t phys_get_total_pages();
uint64_t phys_get_mem_size();

extern uint8_t *phys_bitmap;
extern size_t phys_bitmap_bytes;
extern uint64_t phys_mem_size;
extern uint64_t phys_total_pages;

void init_memory(uint32_t mmap_addr, uint32_t mmap_length);

#ifdef __cplusplus
}
#endif
