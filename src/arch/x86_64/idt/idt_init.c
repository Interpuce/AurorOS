#include <types.h>
#include "idt_structs.h"
#include "pic/pic.h"
#include <panic.h>

static idt_entry_t idt_entries[256];
static idt_ptr_t idt_ptr;

extern uint32_t isr_stub_table[];
extern uint32_t irq_stub_table[];

extern void idt_load(uint32_t); // ./idt_load.asm

void idt_set_gate(uint8_t num, uint64_t base, uint16_t sel, uint8_t flags) {
    idt_entries[num].offset_low  = base & 0xFFFF;
    idt_entries[num].selector    = sel;
    idt_entries[num].ist         = 0;
    idt_entries[num].type_attr   = flags;
    idt_entries[num].offset_mid  = (base >> 16) & 0xFFFF;
    idt_entries[num].offset_high = (base >> 32) & 0xFFFFFFFF;
    idt_entries[num].zero        = 0;
}

void idt_init() {
    pic_remap(0x1f, 0x28);

    idt_ptr.limit = (sizeof(idt_entry_t) * 256) - 1;
    idt_ptr.base = (uint64_t)&idt_entries;

    for (int i = 0; i < 256; ++i) {
        idt_set_gate(i, 0, 0, 0);
    }

    for (int i = 0; i < 32; ++i) {
        idt_set_gate(i, isr_stub_table[i], 0x08, 0x8E);
    }
    
    for (int i = 0; i < 16; ++i) {
        idt_set_gate(32 + i, irq_stub_table[i], 0x08, 0x8E);
    }

    idt_load((uint64_t)&idt_ptr);
}
