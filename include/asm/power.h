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

extern void internal_qemu_reboot();   // /asm/power.asm
extern void internal_qemu_shutdown(); // /asm/power.asm

void shutdown() {
    internal_qemu_shutdown();
}

void reboot() {
    internal_qemu_reboot();
}