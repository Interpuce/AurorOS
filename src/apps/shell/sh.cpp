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

extern "C" int shell_main(uint16_t theme, string current_user);

namespace ShellUtils {
    void printprefix(const char* user, const char* pcname, const char* directory) {
        char home[256];
        int pos = 0;

        if (streql(user, "root")) {
            const char* base = "/root";
            for (int i = 0; base[i] && pos < 255; i++)
                home[pos++] = base[i];
            home[pos] = 0;
        } else {
            const char* base = "/home/";
            for (int i = 0; base[i] && pos < 255; i++)
                home[pos++] = base[i];
            for (int i = 0; user[i] && pos < 255; i++)
                home[pos++] = user[i];
            home[pos] = 0;
        }

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

    namespace EvalCmdReturnType {
        typedef enum {
            Normal,
            ExitShell,
            ExitKernelMainLoop,
            RecursiveNormalExit
        } Type;
    }
    
    EvalCmdReturnType::Type evaluate_command(char* command, uint16_t theme, string current_user, fs_node** current_dir) {
        char *args[10];
        int arg_count = split_str(command, ' ', args, 10);

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
                ShellCommands::cat(*current_dir, args[1]);
            } else if (streql(args[0], "cd")) {
                ShellCommands::cd(current_dir, args[1]);
            } else if (streql(args[0], "ls")) {
                ShellCommands::ls(*current_dir);
            } else if (streql(args[0], "mkdir")) {
                ShellCommands::mkdir(*current_dir, args[1]);
            } else if (streql(args[0], "rm")) {
                ShellCommands::rm(*current_dir, args[1]);
            } else if (streql(args[0], "tinypad")) {
                tinypad_main(0x07, 0x9F);
            } else if (streql(args[0], "help")) {
                ShellCommands::help();
            } else if (streql(args[0], "repo")) {
                println("Visit this link on other device:", 0x07);
                println("https://github.com/Interpuce/AurorOS", 0x07);
            } else if (streql(args[0], "sh")) {
                if (shell_main(theme, current_user) == EvalCmdReturnType::RecursiveNormalExit) return EvalCmdReturnType::RecursiveNormalExit;
            } else if (streql(args[0], "exit")) {
                return EvalCmdReturnType::ExitShell;
            } else if (streql(args[0], "logout")) {
                return EvalCmdReturnType::RecursiveNormalExit;
            } else if (streql(args[0], "kptesting") && AUROR_BETA_STATE != 0) {
                return EvalCmdReturnType::ExitKernelMainLoop;
            } else if (streql(args[0], "eval")) {
                return evaluate_command(farg, theme, current_user, current_dir);
            } else if (streql(args[0], "pwd")) {
                println(ShellCommands::pwd(*current_dir), 0x07);
            } else {
                string error = strcat(args[0], " is neither a known command nor valid AEF binary!");
                print_error(error);
            }
        }
        return EvalCmdReturnType::Normal;
    }
}

extern "C" int shell_main(uint16_t theme, string current_user) {
    char user_home_path[256]; 
    strcpy(user_home_path, "/home/");
    strcat(user_home_path, current_user);
    fs_node* current_dir = fs_resolve(streql(current_user, "root") ? "root" : user_home_path, emulated_fs_root);

    char buffer[128];
    while (KTRUE) {
        ShellUtils::printprefix(current_user, PC_NAME, ShellCommands::pwd(current_dir));
        read_str(buffer, sizeof(buffer), 0, 0x07);

        ShellUtils::EvalCmdReturnType::Type eval_result = ShellUtils::evaluate_command(buffer, theme, current_user, &current_dir);

        using namespace ShellUtils::EvalCmdReturnType;
        if (eval_result == Normal) continue;
        if (eval_result == ExitShell) return 0;
        if (eval_result == RecursiveNormalExit) return RecursiveNormalExit;

        break;
    }

    kernelpanic("KERNEL_MAIN_LOOP_EXITED",
        "This shall not happen unless you're using\n"
        " a developer command 'kptesting' on a beta build of AurorOS.\n"
        " If this screen was not issued by that command, please report\n"
        " an issue."
    );
    return 1;
}

