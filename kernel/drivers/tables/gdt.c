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
#include <asm/gdt.h>

gdt_entry_t gdt[3];
gdtr_t gdtr;

void init_gdt() {
    gdt[0] = (gdt_entry_t){0, 0, 0, 0, 0, 0};
    gdt[1] = (gdt_entry_t){0xFFFF, 0x0000, 0x00, 0x9A, 0xCF, 0x00}; 
    gdt[2] = (gdt_entry_t){0xFFFF, 0x0000, 0x00, 0x92, 0xCF, 0x00};

    gdtr.base = (uint32_t)&gdt;
    gdtr.limit = sizeof(gdt) - 1;
    
    asm volatile("lgdt %0" : : "m" (gdtr));
}