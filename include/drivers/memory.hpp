#pragma once

#include <types.hpp>

extern void* kmalloc(size_t size);
extern void kfree(void* ptr);