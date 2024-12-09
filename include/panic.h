/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#pragma once

#include <screen.h>
#include <types.h>

void kernelpanic(const char *errcode) {
    paintscreen(0x44);
    println(" Kernel panic!", 0x4F);
    println(" ", 0x47);
    printstr(" The system encountered a problem and could not continue! \nYou can turn off the computer manually using the power button.", 0x47);
    println(" ", 0x47);
    printstr(" Error code: ", 0x47);
    printstr(errcode, 0x4F);
    while (1) { __asm__("hlt"); }
}