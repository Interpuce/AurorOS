/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <screen.h>
#include <string.h>
#include <msg.h>

void print_error(const string str) {
    printstr(" [ ", 0x07);
    printstr("ERROR", 0x04);
    printstr(" ] ", 0x07);
    printstr(str, 0x07);
    printstr("\n", 0x07);
}

void print_info(const string str) {
    printstr(" [ ", 0x07);
    printstr("INFO", 0x01);
    printstr(" ] ", 0x07);
    printstr(str, 0x07);
    printstr("\n", 0x07);
}

void print_warn(const string str) {
    printstr(" [ ", 0x07);
    printstr("WARNING", 0x0E);
    printstr(" ] ", 0x07);
    printstr(str, 0x07);
    printstr("\n", 0x07);
}

void print_ok(const string str) {
    printstr(" [ ", 0x07);
    printstr("OK", 0x0B);
    printstr(" ] ", 0x07);
    printstr(str, 0x07);
    printstr("\n", 0x07);
}

void print_custom(const string str, uint8_t color, uint8_t scolor) {
    printstr(" [ ", scolor);
    printstr(str, color);
    printstr(" ] ", scolor);
}

void print(const char* str, uint8_t color) {
    printstr(str, color);
}

void println(const string str, uint8_t color) {
    printstr(str, color);
    printchar('\n', color);
}