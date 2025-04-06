/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <types.h>
#include <drivers/screen.h>

void do_kernel_panic(string code, string* override_devnote) {
    paint_screen(0x04);
    println("AurorOS kernel panic!", 0x07);
    print("  Error code: ", 0x07);
    println(code, 0x07);
    print("  Developer note: ", 0x07);

    if (override_devnote != NULL) {
        println(override_devnote, 0x07);
    } else if (code == "PAGE_FAULT") {
        println("something's probably broken with your memory", 0x07);
    } else {
        println("sorry, not found; try searching for the panic code description", 0x07);
    }

    while (true) {
        asm volatile (
            "cli\n"
            "hlt\n"
        );
    }
}