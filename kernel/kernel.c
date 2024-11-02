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
#include <input.h>
#include <string.h>
#include <types.h>

void main()
{
    string version = "1.0.0 insider preview";
    clear_console();
    print("Welcome to AurorOS!\n", 0x07);
    print("You can type \"help\" for available commands.\n", 0x07);
    run_terminal('user');
    print("Terminal process exited. You can manually turn off your computer.", 0x07);
    while (1) {}
}

void run_terminal(const string user) {
    while (1)
    {
        print("\n", 0x07);
        print(user, 0x05);
        print("@AurorOS > ", 0x05);
  
        string ch = read_string(0x07);
        int count;
        string* charray = split_by_spaces(ch, count);

        if (string_equal(charray[0], "ver") || string_equal(charray[0], "version")) {
            print("AurorOS 1.0.0\n", 0x07);
            print("This version is very unstable and may contain errors and bugs. Let us know on dsc.gg/Auror-OS\n", 0x04);
        } else if (string_equal(charray[0], "clear")) {
            clear_console();
        } else if (string_equal(charray[0], "mush")) {
            if (string_equal(user, "main")) {
                print("mush: cannot request main user access when user already has it\n", 0x04);
                continue;
            }
            print("mush: attempting to start terminal as main user\n", 0x02);
            print("mush: main user password required; default is xxx\n", 0x07);
            if (string_equal(read_string(0x07), "xxx")) {
                run_terminal("main");
                print("mush: terminal process exited; returning to main instance\n", 0x07);
            } 
        } else if (string_equal(charray[0], "quit") || string_equal(charray[0], "exit")) {
            break;
        } else {
            print("Invalid command!\n", 0x04);
        }
    }
}