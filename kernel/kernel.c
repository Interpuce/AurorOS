#include "kernel.h"

#include <string.h>
#include <msg.h>
#include <panic.h>
#include <screen.h>
#include <types.h>
#include <input.h>

#include "commands/commands.h"
#include "envar.h"

void main() {
    clearscreen();

    uint16_t permlvl = 4;
    uint16_t betaState = 1;

    if (betaState==1) {
        print_warn("You are running early build of aurorOS!");
    }
        

    char buffer[128];
    char *args[10];
    while (1) {
        printprefix(usr, pcname);
        readStr(buffer, sizeof(buffer), 0);

        int arg_count = split_str(buffer, ' ', args, 10);

        if (arg_count > 0) {
            char farg[1024] = "";
            for (int i = 1; i < arg_count; i++) {
                strcat(farg, args[i]);
                strcat(farg, " ");
            }

            if (streql(args[0], "ver")) {
                println(version, 0x07);
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
            } else {
                printstr("ERROR ", 0x04);
                printstr(": ", 0x07);
                printstr(args[0], 0x07);
                printstr(" is invalid command! \n", 0x07);
            }
        }
    }

    kernelpanic("KERNEL_MAIN_LOOP_EXITED");
}
