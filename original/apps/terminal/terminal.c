/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <stdio.h>
#include <sys/auth.h>
#include <sys/crash.h>
#include <sys/display.h>
#include <sys/power.h>
#include <string.h>

int tinypad_main(uint8_t color, uint8_t barcolor);

#include "commands/commands.h"

void printprefix(const string user, const string pcname) {
    cprint(" [ ", 0x07);
    cprint(user, 0x09);
    cprint("@", 0x0F);
    cprint(pcname, 0x02);
    cprint(" ]", 0x07);
    cprint(" $ ", 0x0F);
}

int terminal_main(uint16_t theme) {
    cprintln("", 0x07);
    cprintln("                        @@@@@@@@@                           ", theme);                                                            
    cprintln("                       @@@@@@@@@@@@@@                       ", theme);                                                                 
    cprintln("                       @@@@@@@@@@@@@@@@                     ", theme);                                 
    cprintln("                       @@@@@@@@@@@@@@@@                     ", theme);
    cprintln("                    @@@@@@@@@@@@@@@@@@@     @@@@@           ", theme);
    cprintln("                    @@@@@@@@@@@@@@@@@@     @@@@@@@@         ", theme);
    cprintln("                    @@@@@@@@@@@@@@@@      @@@@@@@@@@        ", theme);
    cprintln("                    @@@@@@@@@@@@@@@@     @@@@@@@@@@@        ", theme);
    cprintln("                    @@@@@@@@@@@@@@@@    @@@@@   @@@@@       ", theme);
    cprintln("               @@@@@@@@@@@@@@@@@@@@    @@@@@@   @@@@@@      ", theme);
    cprintln("               @@@@@@@@@@@@@@@@@@     @@@@@@     @@@@@@     ", theme);
    cprintln("                @@@@@@@@@@@@@@@@     @@@@@@       @@@@@     ", theme);
    cprintln("                 @@@@@@@@@@@@@@@    @@@@@@@@@@@@@@@@@@@@    ", theme);
    cprintln("                  @@@@@@@@@@@@     @@@@@@@@@@@@@@@@@@@@@@   ", theme);
    cprintln("                      @@@@@       @@@@@@@@@@@@@@@@@@@@@@@@  ", theme);
    cprintln("                                 @@@@@@             @@@@@@  ", theme);
    cprintln("                                 @@@@@@             @@@@@@  ", theme);
    cprintln("                                  @@@@               @@@@   ", theme);
    cprintln("", 0x07);

    uint8_t beta_state = 2;
    string current_user = "user"; // THIS IS NOT THE AUROROS PERMISSION SYSTEM NOW
    string pc_name = get_pc_name();

    if (beta_state == 1) {
        print_warn("You are using early build of AurorOS!");
    } else if (beta_state == 2) {
        print_warn("You are using public beta build of AurorOS!");
    }

    char buffer[128];
    string args[10];
    while (1) {
        printprefix(current_user, pc_name);
        read(buffer, sizeof(buffer));

        int arg_count = split_str(buffer, ' ', args, 10);

        if (arg_count > 0) {
            char farg[1024] = "";
            for (int i = 1; i < arg_count; i++) {
                strcat(farg, args[i]);
                strcat(farg, " ");
            }

            if (streql(args[0], "ver")) {
                cprint(" ", 0x07);
                cprint(get_auror_name(), 0x07);
                cprint(" ", 0x07);
                cprintln(get_auror_version(), 0x07);
            } else if (streql(args[0], "print")) {
                cprintln(farg, 0x07);
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
            } else if (streql(args[0], "tinypad")) {
                tinypad_main(0x07, 0x9F);
            } else if (streql(args[0], "help")) {
                help();
            } else {
                cprint(" ERROR ", 0x04);
                cprint(": ", 0x07);
                cprint(args[0], 0x07);
                cprint(" is neither a known command nor valid AEF binary! \n", 0x07);
            }
        }
    }

    crash_os();
}
