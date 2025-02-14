/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <sys/display.h>
#include <stdio.h>

int tinypad_main(uint8_t color, uint8_t barcolor) {
    clearscreen();
    printct("Tinypad", barcolor);
    cprintln(" ", 0x07);
    char buffer[1762];
    char *args[10];
    while (1) {
        read(buffer, sizeof(buffer));
        if (streql(buffer, ".exit")) {
            break;
        }
    }
    return 0;
}
