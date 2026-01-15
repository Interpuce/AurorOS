#include <types.hpp>
#include <drivers/memory.hpp>

void* operator new(unsigned int size) {
    return kmalloc(size);
}

void* operator new[](unsigned int size) {
    return kmalloc(size);
}

void operator delete(void* ptr) noexcept {
    kfree(ptr);
}

void operator delete[](void* ptr) noexcept {
    kfree(ptr);
}

void operator delete(void* ptr, size_t) noexcept {
    kfree(ptr);
}

void operator delete[](void* ptr, size_t) noexcept {
    kfree(ptr);
}

void operator delete(void* ptr, unsigned int) noexcept {
    kfree(ptr);
}

void operator delete[](void* ptr, unsigned int) noexcept {
    kfree(ptr);
}