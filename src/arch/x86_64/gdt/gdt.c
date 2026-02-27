#include "gdt_structs.h"

__attribute__((used, section(".data"))) 
volatile struct {
    struct gdt_entry null;
    struct gdt_entry code;
    struct gdt_entry data;
    struct gdt_tss_entry tss;
} __attribute__((packed)) gdt;

struct gdt_ptr gdt_ptr;
struct tss_entry tss_inst; 

extern void gdt_flush(uint64_t);
extern uint64_t _kernel_stack_end;

void gdt_set_gate(int num, uint8_t access, uint8_t gran) {
    struct gdt_entry *entries = (struct gdt_entry*)&gdt;
    entries[num].limit_low = 0;
    entries[num].base_low = 0;
    entries[num].base_middle = 0;
    entries[num].access = access;
    entries[num].granularity = gran;
    entries[num].base_high = 0;
}

void tss_init() {
    uint8_t *ptr = (uint8_t*)&tss_inst;
    for (uint32_t i = 0; i < sizeof(struct tss_entry); i++) ptr[i] = 0;

    tss_inst.rsp0 = (uint64_t)&_kernel_stack_end;
    tss_inst.iomap_base = sizeof(struct tss_entry);
}

void gdt_install() {
    asm volatile ("movq $0xDEADBEEFCAFEBABE, %%rax; movq %%rax, (%0)" 
                 : : "r"(&gdt) : "rax", "memory");

    for(int i = 0; i < sizeof(gdt); i++) ((uint8_t*)&gdt)[i] = 0;

    gdt_set_gate(0, 0, 0);      
    gdt_set_gate(1, 0x9A, 0xAF);     
    gdt_set_gate(2, 0x92, 0xCF);   

    uint64_t tss_base = (uint64_t)&tss_inst;
    uint32_t tss_limit = sizeof(struct tss_entry) - 1;

    gdt.tss.limit_low    = tss_limit & 0xFFFF;
    gdt.tss.base_low     = tss_base & 0xFFFF;
    gdt.tss.base_middle  = (tss_base >> 16) & 0xFF;
    gdt.tss.access       = 0x89; 
    gdt.tss.granularity  = (tss_limit >> 16) & 0x0F;
    gdt.tss.base_high    = (tss_base >> 24) & 0xFF;
    gdt.tss.base_upper   = (tss_base >> 32) & 0xFFFFFFFF;

    gdt_ptr.limit = sizeof(gdt) - 1;
    gdt_ptr.base  = (uint64_t)&gdt;

    asm volatile("" : : : "memory");

    gdt_flush((uint64_t)&gdt_ptr);

    asm volatile("ltr %%ax" : : "a"(0x18));
}
