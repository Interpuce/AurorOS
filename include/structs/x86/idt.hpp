#pragma once
#include <types.hpp>

struct __attribute__((packed)) IDTEntry {
    uint16_t base_low;
    uint16_t sel;
    uint8_t  always0;
    uint8_t  flags;
    uint16_t base_high;
};

struct __attribute__((packed)) IDTPtr {
    uint16_t limit;
    uint32_t base;
};

void idt_init();
void idt_set_gate(uint8_t, uint32_t, uint16_t, uint8_t);

extern void pic_remap();