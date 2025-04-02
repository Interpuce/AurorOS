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

#include <msg.h>
#include <types.h>
#include <screen.h>
#include <ports.h>

void shutdown() {
    outw(0x604, 0x2000); // If shutdown was attempted on QEMU, it should work without issues. If it didn't work, proceed to the screen. However, the FAT32 thing did something with QEMU boot. I don't know. So replacing it with generic screen in case shutdown failed.
    clearscreen();
    paintscreen(0x00);
    print(" \n \n \n \n \n \n \n \n \n Shutting down on your platform has not been implemented yet.\n", 0x06); // TODO: Implement damn real-hardware shutdown.
    println(" ", 0x00);
    print(" System executed an intentional halt instruction in response to the CLI command.\n", 0x06);
    println(" ", 0x00);
    print(" It is now safe to turn your device off manually.", 0x06);
    while (true) {asm("cli; hlt\n");}
}

void reboot() {
    const uint16_t REBOOT_MAGIC1 = 0xfee1;
    const uint16_t REBOOT_MAGIC2 = 0xdead;
    

    clearscreen();
    paintscreen(0x00);
    print(" \n \n \n \n \n \n \n \n \n Rebooting on your platform has not been implemented yet.\n", 0x06); // TODO: Implement real-hardware reboot, too.
    println(" ", 0x00);
    print(" System executed an intentional halt instruction in response to the CLI command.\n", 0x06);
    println(" ", 0x00);
    print(" It is now safe to reset your device manually.", 0x06);

    asm volatile(
        "movw %0, %%ax\n"
        "movw %1, %%bx\n"
        "int $0x19\n"
        :
        : "r"(REBOOT_MAGIC1), "r"(REBOOT_MAGIC2)
    );

    while (true) {asm("cli; hlt\n");} // system clears interrupts flag and halts.

}
