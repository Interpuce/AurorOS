#include <structs/x86/gdt.hpp>

GDTEntry gdt[5];
GDTPtr gdt_ptr;

extern "C" void gdt_flush(uint32_t);

static void gdt_set(
    int i,
    uint32_t base,
    uint32_t limit,
    uint8_t access,
    uint8_t gran
) {
    gdt[i].base_low  = base & 0xFFFF;
    gdt[i].base_mid  = (base >> 16) & 0xFF;
    gdt[i].base_high = (base >> 24) & 0xFF;

    gdt[i].limit_low = limit & 0xFFFF;
    gdt[i].gran = (limit >> 16) & 0x0F;
    gdt[i].gran |= gran & 0xF0;
    gdt[i].access = access;
}

void gdt_init() {
    gdt_ptr.limit = sizeof(gdt) - 1;
    gdt_ptr.base  = (uint32_t)&gdt;

    gdt_set(0, 0, 0, 0, 0);                // null
    gdt_set(1, 0, 0xFFFFFFFF, 0x9A, 0xCF); // kernel code
    gdt_set(2, 0, 0xFFFFFFFF, 0x92, 0xCF); // kernel data
    gdt_set(3, 0, 0xFFFFFFFF, 0xFA, 0xCF); // user code
    gdt_set(4, 0, 0xFFFFFFFF, 0xF2, 0xCF); // user data

    gdt_flush((uint32_t)&gdt_ptr);
}
