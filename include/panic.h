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

#include <msg.h>
#include <types.h>
#include <screen.h>

void kernelpanic(const char *errcode) {
    paintscreen(0x44);
    println(" Kernel panic!", 0x4F);
    println(" ", 0x47);
    print(" The system encountered a problem and could not continue! \nYou can turn off the computer manually using the power button.", 0x47);
    println(" ", 0x47);
    print(" Error code: ", 0x47);
    print(errcode, 0x4F);
    
    while (true) {
        asm("hlt");
    }
}