/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 *
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <types.h>
#include "idt_structs.h"
#include "pic/pic.h"

static idt_entry_t idt_entries[256];
static idt_ptr_t idt_ptr;

extern void idt_load(uint32_t); // ./idt_load.asm

void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt_entries[num].offset_low = (uint16_t)(base & 0xFFFF);
    idt_entries[num].selector = sel;
    idt_entries[num].zero = 0;
    idt_entries[num].type_attr = flags;
    idt_entries[num].offset_high = (uint16_t)((base >> 16) & 0xFFFF);
}

void idt_init() {
    idt_ptr.limit = (sizeof(idt_entry_t) * 256) - 1;
    idt_ptr.base = (uint32_t)&idt_entries;

    for (int i = 0; i < 256; ++i) {
        idt_set_gate(i, 0, 0, 0);
    }

    extern uint32_t isr_stub_table[];
    extern uint32_t irq_stub_table[];

    for (int i = 0; i < 32; ++i) {
        idt_set_gate(i, isr_stub_table[i], 0x08, 0x8E);
    }

    for (int i = 0; i < 16; ++i) {
        idt_set_gate(32 + i, irq_stub_table[i], 0x08, 0x8E);
    }

    pic_remap(0x20, 0x28);

    idt_load((uint32_t)&idt_ptr);
}
