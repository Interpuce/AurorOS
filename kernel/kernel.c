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
#include <constants.h>

extern void run_terminal(const string user);

void kernel_main()
{
    clear_console();
    println("Welcome to AurorOS!", 0x07);
    print("Current version: ", 0x07);
    print(AUROR_VERSION, 0x02);
    if (AUROR_BETA_STATE != 0) {
        if (AUROR_BETA_STATE == 1) {
            print(" beta ", 0x02);
            println(AUROR_SUBVERSION, 0x02);
        }
        if (AUROR_BETA_STATE == 2) {
            print(" alpha ", 0x02);
            println(AUROR_SUBVERSION, 0x02);
        }
    } else {
        println("", 0x07);
    }
    println("You can type \"help\" for available commands.", 0x07);
    run_terminal("user");
    kernel_panic("TOP_SHELL_EXITED");
}
