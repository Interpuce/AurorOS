/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <msg.h>
#include <constants.h>
#include <string.h>
#include <input.h>
#include <asm/power.h>
#include <types.h>
#include <panic.h>
#include <screen.h>
#include <hardware/cpu.h>

#include "commands.h"

void printprefix(const char* user, const char* pcname) {
    print(" [ ", 0x07);
    print(user, 0x09);
    print("@", 0x0F);
    print(pcname, 0x02);
    print(" ]", 0x07);
    print(" $ ", 0x0F);
}

int terminal_main(uint16_t theme) {
    clearscreen();

    println("", 0x07);
    println("                        @@@@@@@@@                           ", theme);                                                            
    println("                       @@@@@@@@@@@@@@                       ", theme);                                                                 
    println("                       @@@@@@@@@@@@@@@@                     ", theme);                                 
    println("                       @@@@@@@@@@@@@@@@                     ", theme);
    println("                    @@@@@@@@@@@@@@@@@@@     @@@@@           ", theme);
    println("                    @@@@@@@@@@@@@@@@@@     @@@@@@@@         ", theme);
    println("                    @@@@@@@@@@@@@@@@      @@@@@@@@@@        ", theme);
    println("                    @@@@@@@@@@@@@@@@     @@@@@@@@@@@        ", theme);
    println("                    @@@@@@@@@@@@@@@@    @@@@@   @@@@@       ", theme);
    println("               @@@@@@@@@@@@@@@@@@@@    @@@@@@   @@@@@@      ", theme);
    println("               @@@@@@@@@@@@@@@@@@     @@@@@@     @@@@@@     ", theme);
    println("                @@@@@@@@@@@@@@@@     @@@@@@       @@@@@     ", theme);
    println("                 @@@@@@@@@@@@@@@    @@@@@@@@@@@@@@@@@@@@    ", theme);
    println("                  @@@@@@@@@@@@     @@@@@@@@@@@@@@@@@@@@@@   ", theme);
    println("                      @@@@@       @@@@@@@@@@@@@@@@@@@@@@@@  ", theme);
    println("                                 @@@@@@             @@@@@@  ", theme);
    println("                                 @@@@@@             @@@@@@  ", theme);
    println("                                  @@@@               @@@@   ", theme);
    println("", 0x07);

    print_info(AUROR_NAME);

    char user[4];
    strcpy(user, "root");

    char buffer[128];
    char *args[10];
    while (1) {
        printprefix(user, PC_NAME);
        read_str(buffer, sizeof(buffer), 0, 0x07);

        int arg_count = split_str(buffer, ' ', args, 10);

        if (arg_count > 0) {
            char farg[1024] = "";
            for (int i = 1; i < arg_count; i++) {
                strcat(farg, args[i]);
                strcat(farg, " ");
            }

            if (streql(args[0], "ver")) {
                print(" ", 0x07);
                print(AUROR_NAME, 0x07);
                print(" ", 0x07);
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
            } else if (streql(args[0], "changeuser")) {
                if (strlen(farg) < 1 || strlen(farg) > 8) {
                    print_error("User name must be between 1 and 8 characters long!");
                } else {
                    strcpy(user, farg);
                }
            } else if (streql(args[0], "cpu")) {
                println(get_cpu_name(), 0x07);
            } else if (streql(args[0], "tinypad")) {
                tinypad_main(0x07, 0x9F);
            } else if (streql(args[0], "debug_panic")) {
                kernelpanic("DEBUG_KERNEL_PANIC");
            } else if (streql(args[0], "help")) {
                help();
            } else {
                printstr(" ERROR ", 0x04);
                printstr(": ", 0x07);
                printstr(args[0], 0x07);
                printstr(" is neither a known command nor valid AEF binary! \n", 0x07);
            }
        }
    }
}

