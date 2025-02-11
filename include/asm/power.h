/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#pragma once

#include <types.h>
#include <ports.h>

void shutdown() {
    outw(0x604, 0x2000);
}

void reboot() {
    const uint16_t REBOOT_MAGIC1 = 0xfee1;
    const uint16_t REBOOT_MAGIC2 = 0xdead;
    
    asm volatile(
        "movw %0, %%ax\n"
        "movw %1, %%bx\n"
        "int $0x19\n"
        :
        : "r"(REBOOT_MAGIC1), "r"(REBOOT_MAGIC2)
    );
}