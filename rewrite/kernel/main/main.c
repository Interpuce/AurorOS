/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <drivers/screen.h>
#include <drivers/keyboard.h>
#include <asm/idt.h>
#include <msg.h>
#include <string.h>

void main() {
    clear_screen();
    print_status(CodeLogTypeOK, "Welcome in AurorOS!");
    print_status(CodeLogTypeOK, "The OS has booted sucessfully.");

    println("", 0x07);
    print_status(CodeLogTypeWarning, "You are on test codebase `project-vernon`, errors may occur");

    while (true) {
        print("# ", 0x07);

        string* input;
        get_input(input, 512, false, 0x07);

        if (streql(input, "test")) {
            print("test", 0x02);
            print("test2", 0x05);
        }

        if (streql(input, "q")) {
            break;
        }
    }

    print_status(CodeLogTypeInfo, "Exited main loop");

    return;
}