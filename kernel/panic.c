/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <types.h>
#include <console.h>

void kernel_panic(const string code) {
    clear_console();
    print_error("KERNEL PANIC!\n");
    println("System occurred a critical error and could not continue.", 0x04);
    println("You can now manually restart/shutdown the computer using power button.", 0x07);
    print("Error code:", 0x0F);
    println(code, 0x07);
    asm("hlt"); // I have no better idea what to do on kernel panic than halting the procesor, eclair25 do this better :)
}