/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <screen.h>
#include <string.h>
#include <msg.h>

void print_error(const char *str) {
    printstr("[  ", 0x07);
    printstr("ERROR", 0x04);
    printstr("  ] ", 0x07);
    printstr(str, 0x07);
    printstr("\n", 0x07);
}

void print_info(const char *str) {
    printstr("[  ", 0x07);
    printstr("INFO", 0x01);
    printstr("  ] ", 0x07);
    printstr(str, 0x07);
    printstr("\n", 0x07);
}

void print_ok(const char *str) {
    printstr("[  ", 0x07);
    printstr("OK", 0x0B);
    printstr("  ] ", 0x07);
    printstr(str, 0x07);
    printstr("\n", 0x07);
}

void print_custom(const char *str, uint8_t color, uint8_t scolor) {
    printstr("[  ", scolor);
    printstr(str, color);
    printstr("  ] ", scolor);
}