#include <structs/x86/idt.hpp>

extern "C" void idt_flush(uint32_t);

IDTEntry idt[256];
IDTPtr idt_ptr;

extern "C" void irq_entry();

extern "C" void irq12_entry();

void idt_set_gate(uint8_t n, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[n].base_low  = base & 0xFFFF;
    idt[n].base_high = (base >> 16) & 0xFFFF;
    idt[n].sel = sel;
    idt[n].always0 = 0;
    idt[n].flags = flags;
}

void idt_init() {
    idt_ptr.limit = sizeof(idt) - 1;
    idt_ptr.base  = (uint32_t)&idt;

    for (int i = 0; i < 256; i++)
        idt_set_gate(i, (uint32_t)irq_entry, 0x08, 0x8E);
    
    idt_set_gate(0x2C, (uint32_t)irq12_entry, 0x08, 0x8E);

    idt_flush((uint32_t)&idt_ptr);
}
