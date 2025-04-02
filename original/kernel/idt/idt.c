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

#define IDT_SIZE 256

struct idt_entry {
    uint16_t base_low;
    uint16_t sel;
    uint8_t always0;
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed));

struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

struct idt_entry idt[IDT_SIZE];
struct idt_ptr idtp;

extern void idt_load(); // can be found in the assembly file
extern void syscall_handler(); // exec/exec.c
extern void rtl_irq_handler(); // net/rtl8139.c

void set_idt_gate(int num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

void idt_install() {
    idtp.limit = (sizeof(struct idt_entry) * IDT_SIZE) - 1;
    idtp.base = (uint32_t)&idt;

    for (int i = 0; i < IDT_SIZE; i++) {
        set_idt_gate(i, 0, 0, 0);
    }

    set_idt_gate(0x80, (uint32_t)syscall_handler, 0x08, 0x8E);
    set_idt_gate(11, (uint32_t)rtl_irq_handler, 0x08, 0x8E);

    idt_load();
}

// gorciu was here and writed this after finding some random article
// about how syscall works from 2006