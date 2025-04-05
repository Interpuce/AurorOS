/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <asm/ports.h>

uint8 inb(uint16 port) {
    uint8 result;
    asm volatile ("inb %1, %0" : "=a" (result) : "Nd" (port));
    return result;
}

void outb(uint16 port, uint8 value) {
    asm volatile ("outb %0, %1" : : "a" (value), "Nd" (port));
}

uint16 inw(uint16 port) {
    uint16 result;
    asm volatile ("inw %1, %0" : "=a" (result) : "Nd" (port));
    return result;
}

void outw(uint16 port, uint16 data) {
    asm volatile ("outw %0, %1" : : "a"(data), "Nd"(port));
}

uint32 inl(uint16 port) {
    uint32 ret;
    asm volatile ("inl %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void outl(uint16 port, uint32 value) {
    asm volatile ("outl %0, %1" :: "a"(value), "Nd"(port));
}