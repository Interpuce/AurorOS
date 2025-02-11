/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <string.h>
#include <screen.h>
#include <msg.h>
#include <types.h>
#include <input.h>

int tinypad_main(uint8_t color, uint8_t barcolor) {
    clearscreen();
    paintline(0, barcolor);
    printct("Tinypad", barcolor);
    println(" ", 0x07);
    char buffer[1762];
    char *args[10];
    while (1) {
        read_str(buffer, sizeof(buffer), 0, color);
        if (streql(buffer, ".exit")) {
            break;
        }
    }
    return 0;
}
