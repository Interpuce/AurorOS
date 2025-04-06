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
#include <asm/idt.h>

extern void idtLoad(); // /asm/idt.asm

void setIdtGate(int num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_low = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

void idtInstall() {
    idtp.limit = (sizeof(struct idtEntry) * IDT_SIZE) - 1;
    idtp.base = (uint32)&idt;

    for (int i = 0; i < IDT_SIZE; i++) {
        setIdtGate(i, 0, 0, 0);
    }

    idtLoad();
}