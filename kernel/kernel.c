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
#include <panic.h>

extern void run_terminal(const string user);

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
