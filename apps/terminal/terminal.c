/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <string.h>
#include <msg.h>
#include <constants.h>
#include <string.h>
#include <input.h>
#include <asm/power.h>

#include "commands/commands.h"

void printprefix(const char* user, const char* pcname) {
    print(" [ ", 0x07);
    print(user, 0x01);
    print("@", 0x0F);
    print(pcname, 0x02);
    print(" ]", 0x07);
    print(" $ ", 0x0F);
}

int terminal_main() {
    clearscreen();
    print("\n", 0x07);

    uint16_t beta_state = true;
    string current_user = "root";

    if (beta_state == true) {
        print_warn("You are running early build of AurorOS!");
    }

    char buffer[128];
    char *args[10];
    while (1) {
        printprefix(current_user, PC_NAME);
        read_str(buffer, sizeof(buffer), 0, 0x07);

        int arg_count = split_str(buffer, ' ', args, 10);

        if (arg_count > 0) {
            char farg[1024] = "";
            for (int i = 1; i < arg_count; i++) {
                strcat(farg, args[i]);
                strcat(farg, " ");
            }

            if (streql(args[0], "ver")) {
                print(AUROR_NAME, 0x07);
                println(AUROR_VERSION, 0x07);
            } else if (streql(args[0], "print")) {
                println(farg ,0x07);
            } else if (streql(args[0], "cowsay")) {
                cowsay(farg);
            } else if (streql(args[0], "map")) {
                map();
            } else if (streql(args[0], "clear")) {
                clearscreen();
            } else if (streql(args[0], "reboot")) {
                reboot();
            } else if (streql(args[0], "shutdown")) {
                shutdown();
            } else if (streql(args[0], "eclair")) {
                eclair(args[1]);
            } else {
                printstr(" ERROR ", 0x04);
                printstr(": ", 0x07);
                printstr(args[0], 0x07);
                printstr(" is invalid command! \n", 0x07);
            }
        }
    }

    kernelpanic("KERNEL_MAIN_LOOP_EXITED");
}
