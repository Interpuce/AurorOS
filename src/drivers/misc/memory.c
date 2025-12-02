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

typedef struct memory_block {
    size_t size;
    struct memory_block *next;
    struct memory_block *prev;
    int free;
} memory_block_t;

#define MEMORY_POOL_SIZE (1024 * 1024)

static uint8_t memory_pool[MEMORY_POOL_SIZE];
static memory_block_t *free_list = (memory_block_t *)memory_pool;
kbool memory_already_initialized = KFALSE;

/*static size_t align_up(size_t value, size_t alignment) {
    return (value + alignment - 1) & ~(alignment - 1);
}*/

void init_memory_heap() {
    free_list->size = MEMORY_POOL_SIZE - sizeof(memory_block_t);
    free_list->next = NULL;
    free_list->prev = NULL;
    free_list->free = 1;
    memory_already_initialized = KTRUE;
}

void *malloc(size_t size) {
    if (!memory_already_initialized) return NULL;
    if (size == 0) return NULL;
    memory_block_t *current = free_list;
    while (current != NULL) {
        if (current->free && current->size >= size) {
            if (current->size > size + sizeof(memory_block_t)) {
                memory_block_t *new_block = (void *)((uint8_t *)current + sizeof(memory_block_t) + size);
                new_block->size = current->size - size - sizeof(memory_block_t);
                new_block->next = current->next;
                new_block->prev = current;
                new_block->free = 1;
                if (current->next)
                    current->next->prev = new_block;
                current->next = new_block;
            }
            current->size = size;
            current->free = 0;
            return (uint8_t *)current + sizeof(memory_block_t);
        }
        current = current->next;
    }
    return NULL;
}

void free(void *ptr) {
    if (!ptr || !memory_already_initialized) return;
    memory_block_t *block = (void *)((uint8_t *)ptr - sizeof(memory_block_t));
    if (block->free) return;
    block->free = 1;
    if (block->next && block->next->free) {
        block->size += sizeof(memory_block_t) + block->next->size;
        block->next = block->next->next;
        if (block->next) block->next->prev = block;
    }
    if (block->prev && block->prev->free) {
        block->prev->size += sizeof(memory_block_t) + block->size;
        block->prev->next = block->next;
        if (block->next) block->next->prev = block->prev;
    }
}

void *malloc_aligned(size_t size, size_t alignment) {
    size_t total = size + alignment + sizeof(void*);
    void *raw = malloc(total);
    if (!raw) return NULL;
    uintptr_t aligned = ((uintptr_t)raw + sizeof(void*) + alignment - 1) & ~(alignment - 1);
    void **store = (void**)(aligned - sizeof(void*));
    *store = raw;
    return (void*)aligned;
}

/* minimal memcpy/memmove/memset */
void *memcpy(void *dest, const void *src, size_t n) {
    uint8_t *d = dest;
    const uint8_t *s = src;
    for (size_t i = 0; i < n; i++) d[i] = s[i];
    return dest;
}

void *memmove(void *dest, const void *src, size_t n) {
    uint8_t *d = dest;
    const uint8_t *s = src;
    if (d == s) return dest;
    if (d < s) {
        for (size_t i = 0; i < n; i++) d[i] = s[i];
    } else {
        for (size_t i = n; i > 0; i--) d[i - 1] = s[i - 1];
    }
    return dest;
}

void *memset(void *ptr, int value, size_t num) {
    uint8_t *dst = ptr;
    for (size_t i = 0; i < num; i++) dst[i] = (uint8_t)value;
    return ptr;
}

/* physical memory manager */

/* short: multiboot mmap entry */
typedef struct __attribute__((packed)) {
    uint32_t size;
    uint64_t addr;
    uint64_t len;
    uint32_t type;
} multiboot_mmap_entry_t;

uint8_t *phys_bitmap = NULL;
size_t phys_bitmap_bytes = 0;
uint64_t phys_mem_size = 0;
uint64_t phys_total_pages = 0;

/* early bump allocator pointers */
static uint64_t early_alloc_phys_ptr = 0;
static uint64_t early_alloc_phys_end = 0;

/* external mapping helper - implement in platform */
static inline void* phys_to_virt(uint32_t phys_addr) {
    return (void*)phys_addr;
}

/*static inline uint64_t virt_to_phys(void* virt_addr) {
    return (uint64_t)virt_addr;
}*/

/* bitmap helpers */
static inline void bitmap_set_bit(size_t bit_index) {
    phys_bitmap[bit_index >> 3] |= (1u << (bit_index & 7));
}
static inline void bitmap_clear_bit(size_t bit_index) {
    phys_bitmap[bit_index >> 3] &= ~(1u << (bit_index & 7));
}
static inline int bitmap_test_bit(size_t bit_index) {
    return (phys_bitmap[bit_index >> 3] >> (bit_index & 7)) & 1;
}

void mark_page_used(uint64_t page_index) {
    if (page_index >= phys_total_pages) return;
    bitmap_set_bit((size_t)page_index);
}
void mark_page_free(uint64_t page_index) {
    if (page_index >= phys_total_pages) return;
    bitmap_clear_bit((size_t)page_index);
}
int page_is_free(uint64_t page_index) {
    if (page_index >= phys_total_pages) return 0;
    return !bitmap_test_bit((size_t)page_index);
}

static inline uint64_t round_up_to_page(uint64_t x) {
    return (x + PM_PAGE_SIZE - 1) & ~(PM_PAGE_SIZE - 1);
}
static inline uint64_t round_down_to_page(uint64_t x) {
    return x & ~(PM_PAGE_SIZE - 1);
}

uint64_t early_phys_alloc_phys(size_t size, size_t alignment) {
    if (size == 0) return 0;
    if (alignment == 0) alignment = 1;
    uint64_t ptr = (early_alloc_phys_ptr + alignment - 1) & ~(alignment - 1);
    if (ptr + size > early_alloc_phys_end) return 0;
    uint64_t ret = ptr;
    early_alloc_phys_ptr = ptr + size;
    return ret;
}

void *early_phys_alloc_virtual(size_t size, size_t alignment) {
    uint64_t phys = early_phys_alloc_phys(size, alignment);
    if (!phys) return NULL;
    return phys_to_virt(phys);
}

void physmem_init_from_multiboot(uint32_t mmap_addr, uint32_t mmap_length) {
    if (!mmap_addr || mmap_length == 0) return;
    uint32_t cur = mmap_addr;
    uint64_t highest = 0;
    while (cur < mmap_addr + mmap_length) {
        multiboot_mmap_entry_t *e = (multiboot_mmap_entry_t *)cur;
        uint64_t end = e->addr + e->len;
        if (end > highest) highest = end;
        cur += e->size + sizeof(e->size);
    }
    phys_mem_size = round_up_to_page(highest);
    phys_total_pages = phys_mem_size / PM_PAGE_SIZE;
    phys_bitmap_bytes = (phys_total_pages + 7) / 8;
    uint64_t bitmap_phys = 0;
    int found_region = 0;
    if (early_alloc_phys_end > early_alloc_phys_ptr) {
        uint64_t candidate = early_phys_alloc_phys(phys_bitmap_bytes, PM_PAGE_SIZE);
        if (candidate) { bitmap_phys = candidate; found_region = 1; }
    }
    if (!found_region) {
        cur = mmap_addr;
        while (cur < mmap_addr + mmap_length) {
            multiboot_mmap_entry_t *e = (multiboot_mmap_entry_t *)cur;
            if (e->type == 1) {
                uint64_t start = round_up_to_page(e->addr);
                uint64_t end = round_down_to_page(e->addr + e->len);
                uint64_t region_bytes = (end > start) ? (end - start) : 0;
                if (region_bytes >= phys_bitmap_bytes + PM_PAGE_SIZE) {
                    bitmap_phys = start;
                    found_region = 1;
                    break;
                }
            }
            cur += e->size + sizeof(e->size);
        }
    }
    if (!found_region) {
        if (phys_mem_size > phys_bitmap_bytes + PM_PAGE_SIZE) {
            bitmap_phys = phys_mem_size - phys_bitmap_bytes - PM_PAGE_SIZE;
            found_region = 1;
        }
    }
    if (!found_region) { phys_bitmap = NULL; phys_bitmap_bytes = 0; phys_total_pages = 0; phys_mem_size = 0; return; }
    phys_bitmap = (uint8_t *)phys_to_virt(bitmap_phys);
    if (!phys_bitmap) phys_bitmap = (uint8_t *)(uintptr_t)bitmap_phys;
    for (size_t i = 0; i < phys_bitmap_bytes; i++) phys_bitmap[i] = 0xFF;
    uint64_t bitmap_first_page = bitmap_phys / PM_PAGE_SIZE;
    uint64_t bitmap_pages = (phys_bitmap_bytes + PM_PAGE_SIZE - 1) / PM_PAGE_SIZE;
    for (uint64_t p = 0; p < bitmap_pages; p++) mark_page_used(bitmap_first_page + p);
    cur = mmap_addr;
    while (cur < mmap_addr + mmap_length) {
        multiboot_mmap_entry_t *e = (multiboot_mmap_entry_t *)cur;
        if (e->type == 1) {
            uint64_t start = round_up_to_page(e->addr);
            uint64_t end = round_down_to_page(e->addr + e->len);
            for (uint64_t p = start; p < end; p += PM_PAGE_SIZE) {
                uint64_t idx = p / PM_PAGE_SIZE;
                if (idx >= phys_total_pages) break;
                if (p >= bitmap_phys && p < bitmap_phys + bitmap_pages * PM_PAGE_SIZE) continue;
                bitmap_clear_bit((size_t)idx);
            }
        }
        cur += e->size + sizeof(e->size);
    }
    //uint64_t bump_base = 0;
    cur = mmap_addr;
    while (cur < mmap_addr + mmap_length) {
        multiboot_mmap_entry_t *e = (multiboot_mmap_entry_t *)cur;
        if (e->type == 1) {
            uint64_t start = round_up_to_page(e->addr);
            uint64_t end = round_down_to_page(e->addr + e->len);
            if (bitmap_phys >= start && bitmap_phys < end) {
                uint64_t candidate_bump = round_up_to_page(bitmap_phys + bitmap_pages * PM_PAGE_SIZE);
                if (candidate_bump + PM_PAGE_SIZE <= end) { early_alloc_phys_ptr = candidate_bump; early_alloc_phys_end = end; break; }
            } else if (start > bitmap_phys) { early_alloc_phys_ptr = start; early_alloc_phys_end = end; break; }
        }
        cur += e->size + sizeof(e->size);
    }
    if (early_alloc_phys_ptr == 0) {
        early_alloc_phys_ptr = round_up_to_page(bitmap_phys + bitmap_pages * PM_PAGE_SIZE);
        early_alloc_phys_end = phys_mem_size;
        if (early_alloc_phys_ptr >= early_alloc_phys_end) { early_alloc_phys_ptr = early_alloc_phys_end = 0; }
    }
}

uint64_t phys_alloc_contiguous(size_t size, size_t alignment) {
    if (phys_bitmap == NULL) return 0;
    if (size == 0) return 0;
    if (alignment == 0) alignment = PM_PAGE_SIZE;
    if ((alignment & (alignment - 1)) != 0) return 0;
    uint64_t pages = (size + PM_PAGE_SIZE - 1) / PM_PAGE_SIZE;
    uint64_t align_pages = alignment / PM_PAGE_SIZE;
    if (align_pages == 0) align_pages = 1;
    uint64_t total_pages = phys_total_pages;
    for (uint64_t i = 0; i + pages <= total_pages; i++) {
        if ((i & (align_pages - 1)) != 0) continue;
        if (!page_is_free(i)) continue;
        uint64_t count = 1;
        while (count < pages && page_is_free(i + count)) count++;
        if (count == pages) {
            for (uint64_t j = 0; j < pages; j++) mark_page_used(i + j);
            return i * PM_PAGE_SIZE;
        }
        i += count;
    }
    return 0;
}

void phys_free_contiguous(uint64_t phys_addr, size_t size) {
    if (phys_addr == 0 || size == 0) return;
    uint64_t first_page = phys_addr / PM_PAGE_SIZE;
    uint64_t pages = (size + PM_PAGE_SIZE - 1) / PM_PAGE_SIZE;
    for (uint64_t i = 0; i < pages; i++) {
        uint64_t idx = first_page + i;
        if (idx < phys_total_pages) bitmap_clear_bit((size_t)idx);
    }
}

uint64_t phys_get_total_pages() { return phys_total_pages; }
uint64_t phys_get_mem_size() { return phys_mem_size; }

void init_memory(uint32_t mmap_addr, uint32_t mmap_length) {
    init_memory_heap();
    physmem_init_from_multiboot(mmap_addr, mmap_length);
}