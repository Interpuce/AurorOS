#include <types.h>
#include "gdt_structs.h"

struct gdt_entry gdt[4];
struct gdt_ptr gdt_ptr;
struct tss_entry tss;

extern void gdt_flush(uint32_t);

void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    gdt[num].base_low    = base & 0xFFFF;
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high   = (base >> 24) & 0xFF;
    gdt[num].limit_low   = limit & 0xFFFF;
    gdt[num].granularity = (limit >> 16) & 0x0F;
    gdt[num].granularity |= gran & 0xF0;
    gdt[num].access      = access;
}

extern uint32_t _kernel_stack_end;

void tss_init() {
    for (unsigned int i = 0; i < sizeof(struct tss_entry)/4; i++)
        ((uint32_t*)&tss)[i] = 0;

    tss.esp0 = (uint32_t)&_kernel_stack_end;
    tss.ss0  = 0x10; // kernel data segment
    tss.iomap_base = sizeof(struct tss_entry);
}

void gdt_install() {
    gdt_ptr.limit = sizeof(gdt) - 1;
    gdt_ptr.base  = (uint32_t)&gdt;

    gdt_set_gate(0, 0, 0, 0, 0);        // null
    gdt_set_gate(1, 0, 0xFFFFF, 0x9A, 0xCF); // code
    gdt_set_gate(2, 0, 0xFFFFF, 0x92, 0xCF); // data
    gdt_set_gate(3, (uint32_t)&tss, sizeof(struct tss_entry)-1, 0x89, 0x00); // TSS

    gdt_flush((uint32_t)&gdt_ptr);
}
