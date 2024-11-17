/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <screen.h>
#include <types.h>
#include <msghandler.h>

extern void run_terminal(const string user);

void kernel_panic(const string code) {
    clear_console();
    print_error("KERNEL PANIC!\n");
    println("System occurred a critical error and could not continue.", 0x04);
    println("You can now manually restart/shutdown the computer using power button.", 0x07);
    print("Error code:", 0x0F);
    println(code, 0x07);
    asm("hlt"); // i have no better idea what to do on kernel panic than halting the procesor, eclair25 do this better :)
}

void kernel_main()
{
    string version = "1.0.0 [private beta]";
    clear_console();
    println("Welcome to AurorOS!", 0x07);
    print("Current version: ", 0x07);
    println(version, 0x02);
    println("You can type \"help\" for available commands.", 0x07);
    run_terminal("user");
    kernel_panic("TOP_SHELL_EXITED");
}
