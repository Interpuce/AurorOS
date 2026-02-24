#pragma once

#include <types.h>
#include <ports.h>

uint16_t get_cursor() {
    outb(0x3D4, 0x0E);
    uint16_t high = inb(0x3D5);
    outb(0x3D4, 0x0F);
    uint16_t low = inb(0x3D5);
    return (high << 8) | low;
}

void set_cursor(uint16_t pos) {
    outb(0x3D4, 0x0E);
    outb(0x3D5, (pos >> 8) & 0xFF);
    outb(0x3D4, 0x0F);
    outb(0x3D5, pos & 0xFF);
}

void move_cursor_left() {
    uint16_t pos = get_cursor();
    if (pos > 0) set_cursor(pos - 1);
}

void move_cursor_right() {
    uint16_t pos = get_cursor();
    set_cursor(pos + 1);
}
