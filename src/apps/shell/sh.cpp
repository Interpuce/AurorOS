/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#pragma GCC optimize ("O3")

extern "C" {
    #include <string.h>
    #include <msg.h>
    #include <constants.h>
    #include <string.h>
    #include <input.h>
    #include <asm/power.h>
    #include <panic.h>
    #include <fs-emulated.h>
    #include <filesystem.h>
}

#include "commands/commands.hpp"
#include <apps/tinypad.hpp>

void printprefix(const char* user, const char* pcname, const char* directory) {
    char home[256];
    int pos = 0;

    const char* base = "/home/";
    for (int i = 0; base[i] && pos < 255; i++) home[pos++] = base[i];
    for (int i = 0; user[i] && pos < 255; i++) home[pos++] = user[i];
    home[pos] = 0;

    const char* shown = directory;
    int match = 1;

    for (int i = 0; home[i]; i++) {
        if (directory[i] != home[i]) {
            match = 0;
            break;
        }
    }

    if (match) {
        shown = "~";
    }

    print(user, 0x0B);
    print("@", 0x07);
    print(pcname, 0x0B);
    print(":", 0x07);
    print(shown, 0x0B);
    print(" $ ", 0x0F);
}

char* num2str(int value, char* buffer) {
    char temp[32];
    int pos = 0;
    int neg = value < 0;

    if (value == 0) {
        buffer[0] = '0';
        buffer[1] = '\0';
        return buffer;
    }

    if (neg) value = -value;

    while (value > 0) {
        temp[pos++] = '0' + (value % 10);
        value /= 10;
    }

    int i = 0;

    if (neg) buffer[i++] = '-';

    while (pos > 0) {
        buffer[i++] = temp[--pos];
    }

    buffer[i] = '\0';
    return buffer;
}

extern "C" int shell_main(uint16_t theme, string current_user) {
    fs_node* current_dir = fs_resolve("/home/liveuser", emulated_fs_root);

    uint8_t beta_state = AUROR_BETA_STATE;

    if (beta_state == 1) {
        print_warn("You are using early build of AurorOS!");
    } else if (beta_state == 2) {
        print_warn("You are using public beta build of AurorOS!");
    } else if (beta_state == 3) {
        print_warn("You are using release candidate build of AurorOS!");
    } else {
        print_ok("You're running a stable version of AurorOS!");
    }

    char buffer[128];
    char *args[10];
    while (1) {
        printprefix(current_user, PC_NAME, ShellCommands::pwd(current_dir));
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
                ShellCommands::cowsay(farg);
            } else if (streql(args[0], "map")) {
                ShellCommands::map();
            } else if (streql(args[0], "clear")) {
                clearscreen();
            } else if (streql(args[0], "reboot")) {
                reboot();
            } else if (streql(args[0], "shutdown")) {
                shutdown();
            } else if (streql(args[0], "eclair")) {
                ShellCommands::eclair(args[1]);
            } else if (streql(args[0], "cat")) {
                ShellCommands::cat(current_dir, args[1]);
            } else if (streql(args[0], "cd")) {
                ShellCommands::cd(&current_dir, args[1]);
            } else if (streql(args[0], "ls")) {
                ShellCommands::ls(current_dir);
            } else if (streql(args[0], "mkdir")) {
                ShellCommands::mkdir(current_dir, args[1]);
            } else if (streql(args[0], "rm")) {
                ShellCommands::rm(current_dir, args[1]);
            } else if (streql(args[0], "tinypad")) {
                tinypad_main(0x07, 0x9F);
            } else if (streql(args[0], "help")) {
                ShellCommands::help();
            } else if (streql(args[0], "repo")) {
                println("Visit this link on other device:", 0x07);
                println("https://github.com/Interpuce/AurorOS", 0x07);
            } else if (streql(args[0], "sh")) {
                shell_main(theme, current_user);
            } else if (streql(args[0], "exit")) {
                return 0;
            } else if (streql(args[0], "kptesting") && AUROR_BETA_STATE != 0) {
                break;
            } else {
                string error = strcat(args[0], " is neither a known command nor valid AEF binary!");
                print_error(error);
            }
        }
    }

    kernelpanic("KERNEL_MAIN_LOOP_EXITED",
        "This shall not happen unless you're using\n"
        " a developer command 'kptesting' on a beta build of AurorOS.\n"
        " If this screen was not issued by that command, please report\n"
        " an issue."
    );
    return 1;
}

