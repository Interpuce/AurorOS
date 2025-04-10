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
#include <drivers/memory.h>
#include <asm/idt.h>

extern void idt_load(); // /asm/idt.asm
extern void syscall_stub();

void syscall_handler() {
    return;
}

void set_idt_gate(int num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

void idt_install() {
    idtp.limit = (sizeof(struct idt_entry) * IDT_SIZE) - 1;
    idtp.base = (uint32)&idt;

    for (int i = 0; i < IDT_SIZE; i++) {
        set_idt_gate(i, 0, 0, 0);
    }

    set_idt_gate(14, (uint32_t)page_fault_handler, 0x08, 0x8E);
    set_idt_gate(0x80, (uint32_t)syscall_stub, 0x23, 0xEE);

    idt_load();
}