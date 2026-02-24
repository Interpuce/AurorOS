#pragma once

#include <types.h>

static inline void io_wait() {
    __asm__ volatile ("outb %%al, $0x80" : : "a"(0));
}

static inline void sleep_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms; i++) {
        for (volatile uint32_t j = 0; j < 100000; j++) {
            __asm__ volatile("nop");
        }
        io_wait(); 
    }
}
