/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#ifndef INCLUDE_SYSTEM
#define INCLUDE_SYSTEM 1

#include <types.h>

/**
 * inb vs inw, outb vs outw - what's the difference?
 * inb (in port byte) works in 8-bits while inw (in port word) works in 16-bits
 * outb (out port byte) works in 8-bits while outw (out port word) works in 16-bits
 */

uint8 inb(uint16 _port) {
    uint8 rv;
    asm volatile ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

void outb(uint16 _port, uint8 _data) {
	asm volatile ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

uint16_t inw(uint16_t port) {
    uint16_t value;
    asm volatile ("inw %1, %0" : "=a"(value) : "d"(port));
    return value;
}

void outw(uint16_t port, uint16_t value) {
    asm volatile ("outw %0, %1" : : "a"(value), "d"(port));
}

#endif