#include <types.hpp>

extern uint8_t _kernel_stack_end; 
static uint8_t* heap_ptr = &_kernel_stack_end;

void* kmalloc(size_t size) {
    size = (size + 3) & ~3;
    void* ptr = heap_ptr;
    heap_ptr += size;
    return ptr;
}

void kfree(void* ptr) {
    (void)ptr;
}