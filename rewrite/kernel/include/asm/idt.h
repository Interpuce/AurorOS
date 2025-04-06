/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#pragma once

#include <types.h>

#define IDT_SIZE 256

struct idtEntry {
    uint16 base_low;
    uint16 sel;
    uint8 always0;
    uint8 flags;
    uint16 base_high;
} __attribute__((packed));

struct idtPtr {
    uint16 limit;
    uint32 base;
} __attribute__((packed));

struct idtEntry idt[IDT_SIZE];
struct idtPtr idtp;

extern void idt_install();