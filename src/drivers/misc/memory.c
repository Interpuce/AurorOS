#include <types.h>

typedef struct memory_block {
    size_t size;
    struct memory_block *next;
    struct memory_block *prev;
    int64_t free;
} __attribute__((aligned(16))) memory_block_t;

#define MEMORY_POOL_SIZE (1024 * 1024)
#define PM_PAGE_SIZE 4096

static uint8_t memory_pool[MEMORY_POOL_SIZE] __attribute__((aligned(16)));
static memory_block_t *free_list = (memory_block_t *)memory_pool;
int memory_already_initialized = KFALSE;

typedef struct __attribute__((packed)) {
    uint32_t size;
    uint64_t addr;
    uint64_t len;
    uint32_t type;
    uint32_t reserved;
} multiboot_mmap_entry_t;

uint8_t *phys_bitmap = NULL;
size_t phys_bitmap_bytes = 0;
uint64_t phys_mem_size = 0;
uint64_t phys_total_pages = 0;

static uint64_t early_alloc_phys_ptr = 0;
static uint64_t early_alloc_phys_end = 0;

static inline void* phys_to_virt(uint64_t phys_addr) {
    return (void*)phys_addr; 
}

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
    return (x + PM_PAGE_SIZE - 1) & ~((uint64_t)PM_PAGE_SIZE - 1);
}

static inline uint64_t round_down_to_page(uint64_t x) {
    return x & ~((uint64_t)PM_PAGE_SIZE - 1);
}

void init_memory_heap() {
    free_list->size = MEMORY_POOL_SIZE - sizeof(memory_block_t);
    free_list->next = NULL;
    free_list->prev = NULL;
    free_list->free = 1;
    memory_already_initialized = KTRUE;
}

void *malloc(size_t size) {
    if (!memory_already_initialized || size == 0) return NULL;
    
    size = (size + 15) & ~15;
    memory_block_t *current = free_list;
    
    while (current != NULL) {
        if (current->free && current->size >= size) {
            if (current->size > size + sizeof(memory_block_t) + 16) {
                memory_block_t *new_block = (memory_block_t *)((uint8_t *)current + sizeof(memory_block_t) + size);
                new_block->size = current->size - size - sizeof(memory_block_t);
                new_block->next = current->next;
                new_block->prev = current;
                new_block->free = 1;
                
                if (current->next) current->next->prev = new_block;
                current->next = new_block;
                current->size = size;
            }
            current->free = 0;
            return (void *)((uint8_t *)current + sizeof(memory_block_t));
        }
        current = current->next;
    }
    return NULL;
}

void free(void *ptr) {
    if (!ptr || !memory_already_initialized) return;
    
    memory_block_t *block = (memory_block_t *)((uint8_t *)ptr - sizeof(memory_block_t));
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

void *memcpy(void *dest, const void *src, size_t n) {
    uint8_t *d = dest;
    const uint8_t *s = src;
    for (size_t i = 0; i < n; i++) d[i] = s[i];
    return dest;
}

void *memset(void *ptr, int value, size_t num) {
    uint8_t *dst = ptr;
    for (size_t i = 0; i < num; i++) dst[i] = (uint8_t)value;
    return ptr;
}

uint64_t early_phys_alloc_phys(size_t size, size_t alignment) {
    if (size == 0) return 0;
    uint64_t ptr = (early_alloc_phys_ptr + alignment - 1) & ~(alignment - 1);
    if (ptr + size > early_alloc_phys_end) return 0;
    uint64_t ret = ptr;
    early_alloc_phys_ptr = ptr + size;
    return ret;
}

void physmem_init_from_multiboot(uint64_t mmap_addr, uint32_t mmap_length) {
    uint64_t cur = mmap_addr;
    uint64_t highest = 0;
    
    while (cur < mmap_addr + mmap_length) {
        multiboot_mmap_entry_t *e = (multiboot_mmap_entry_t *)cur;
        if (e->addr + e->len > highest) highest = e->addr + e->len;
        cur += e->size + 4;
    }
    
    phys_mem_size = round_up_to_page(highest);
    phys_total_pages = phys_mem_size / PM_PAGE_SIZE;
    phys_bitmap_bytes = (phys_total_pages + 7) / 8;
    
    uint64_t bitmap_phys = 0;
    cur = mmap_addr;
    while (cur < mmap_addr + mmap_length) {
        multiboot_mmap_entry_t *e = (multiboot_mmap_entry_t *)cur;
        if (e->type == 1 && e->len >= phys_bitmap_bytes) {
            bitmap_phys = round_up_to_page(e->addr);
            break;
        }
        cur += e->size + 4;
    }
    
    phys_bitmap = (uint8_t *)phys_to_virt(bitmap_phys);
    for (size_t i = 0; i < phys_bitmap_bytes; i++) phys_bitmap[i] = 0xFF;
    
    cur = mmap_addr;
    while (cur < mmap_addr + mmap_length) {
        multiboot_mmap_entry_t *e = (multiboot_mmap_entry_t *)cur;
        if (e->type == 1) {
            uint64_t start = round_up_to_page(e->addr);
            uint64_t end = round_down_to_page(e->addr + e->len);
            for (uint64_t p = start; p < end; p += PM_PAGE_SIZE) {
                if (p < bitmap_phys || p >= bitmap_phys + phys_bitmap_bytes)
                    bitmap_clear_bit(p / PM_PAGE_SIZE);
            }
        }
        cur += e->size + 4;
    }
}

uint64_t phys_alloc_contiguous(size_t size, size_t alignment) {
    if (!phys_bitmap) return 0;
    uint64_t pages = (size + PM_PAGE_SIZE - 1) / PM_PAGE_SIZE;
    uint64_t align_pages = alignment / PM_PAGE_SIZE;
    if (align_pages == 0) align_pages = 1;
    
    for (uint64_t i = 0; i + pages <= phys_total_pages; i += align_pages) {
        int found = 1;
        for (uint64_t j = 0; j < pages; j++) {
            if (!page_is_free(i + j)) { found = 0; break; }
        }
        if (found) {
            for (uint64_t j = 0; j < pages; j++) mark_page_used(i + j);
            return i * PM_PAGE_SIZE;
        }
    }
    return 0;
}

void init_memory(uint64_t mmap_addr, uint32_t mmap_length) {
    init_memory_heap();
    physmem_init_from_multiboot(mmap_addr, mmap_length);
}
