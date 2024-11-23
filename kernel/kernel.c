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
#include <declarations/panic.h>
#include <constants.h>

extern int run_terminal(const string user);
extern int start_gui(const string user);

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
            println("This version is very unstable and may contain errors and bugs. Let us know on dsc.gg/Auror-OS", 0x04);
        }
    } else {
        println("", 0x07);
    }
    println("You can type \"help\" for available commands.", 0x07);
    int terminalexitcode = run_terminal("user");
    if (terminalexitcode != 1) {
        kernel_panic("TOP_SHELL_EXITED");
    } else {
        start_gui("main");
        kernel_panic("GUI_ERROR");
    }
}
