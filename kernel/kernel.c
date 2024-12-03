#include "kernel.h"

#include <string.h>
#include <msg.h>
#include <panic.h>
#include <screen.h>
#include <types.h>

#include "commands/commands.h"

void main() {
    clearscreen();
    print_ok("Hello, world!");
    const char *version = "AurorOS 0.0.1 (BETA)";

    const char *usr = "root";
    const char *pcname = "auror";
    print_custom("INFO", 0x0A, 0x07);
    printstr("logged in as ", 0x07);
    printstr(usr, 0x07);
    printstr("\n", 0x07);

    uint16_t permlvl = 4;
    print_custom("INFO", 0x0A, 0x07);
    printstr("Permission level is set to ", 0x07);
    printint(permlvl, 0x07);
    printstr("\n", 0x07);

    char buffer[128];
    char *args[10];
    while (1) {
        printprefix(usr, pcname);
        read_str(buffer, sizeof(buffer), 0);

        int arg_count = split_str(buffer, ' ', args, 10);

        if (arg_count > 0) {
            char farg[1024] = "";
            for (int i = 1; i < arg_count; i++) {
                strcat(farg, args[i]);
                strcat(farg, " ");
            }

            if (streql(args[0], "ver")) {
                println(version, 0x07);
            } else if (streql(args[0], "passw")) {
                read_str(buffer, sizeof(buffer), 1);
            } else if (arg_count > 0 && streql(args[0], "print")) {
                println(farg ,0x07);
            } else if (arg_count > 0 && streql(args[0], "cowsay")) {
                cowsay(farg);
            } else if (arg_count > 0 && streql(args[0], "map")) {
                map();
            } else {
                print_error("Invalid command");
            }
        }
    }

    kernelpanic("KERNEL_MAIN_LOOP_EXITED");
}
