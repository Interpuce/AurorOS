/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <msg.h>
#include <types.h>
#include <screen.h>
#include <panic.h>

void kernelpanic(const char *errcode) {
    paintscreen(COLOR_3);
    println(" ", COLOR_1);
    println(" Kernel panic!", COLOR_2);
    println(" ", COLOR_1);
    print(" The system encountered a problem and could not continue! \n You can turn off the computer manually using the power button.", COLOR_1);
    println(" ", COLOR_1);
    print(" Error code: ", COLOR_1);
    print(errcode, COLOR_2);

    while (true) {
        asm("hlt");
    }
}
