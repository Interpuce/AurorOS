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
    outw(0x604, 0x2000); // If shutdown was attempted on QEMU, it should work without issues. If it didn't work, proceed to the screen
    clearscreen();
    paintscreen(0x00);
    print(" \n \n \n \n \n \n \n \n \n Shutting down outside of QEMU has not been implemented yet.\n", 0x06); // TODO: Implement damn real-hardware shutdown.
    println(" ", 0x00);
    print(" System executed an intentional halt instruction in response to the CLI command.\n", 0x06);
    println(" ", 0x00);
    print(" It is now safe to turn your device off manually.", 0x06);
    while (true) {asm("cli; hlt\n");}
}

void reboot() {
    const uint16_t REBOOT_MAGIC1 = 0xfee1;
    const uint16_t REBOOT_MAGIC2 = 0xdead;
    
/*
Technically, the "not supported outside QEMU" screen shouldn't be visible on QEMU because the reboot is very rapid (at least as for AurorOS version 1.0.1). But it could theoretically show if the host machine is weak or running at its limits...
We don't want that screen to display for QEMU users, since it's only intended for non-QEMU users.
I would move it under QEMU reboot ASM logic, but it doesn't show on real hardware then.
TODO: Write a better implementation that will migitate the risk of this stuff displaying on weaker-host QEMU VMs.
*/

    clearscreen();
    paintscreen(0x00);
    print(" \n \n \n \n \n \n \n \n \n Rebooting outside of QEMU has not been implemented yet.\n", 0x06); // TODO: Implement real-hardware reboot, too.
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

    while (true) {asm("cli; hlt\n");} // If reboot attempt was made on QEMU, the VM has already rebooted. If it's on real hardware and QEMU reboot is uneffective, system clears interrupts flag and halts.

}
