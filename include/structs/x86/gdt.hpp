#pragma once

#include <types.hpp>
#include <compiler.hpp>

struct ATTRIBUTE_PACKED GDTEntry {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_mid;
    uint8_t  access;
    uint8_t  gran;
    uint8_t  base_high;
};

struct ATTRIBUTE_PACKED GDTPtr {
    uint16_t limit;
    uint32_t base;
};

void gdt_init();
