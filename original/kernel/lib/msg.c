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
#include <screen.h>
#include <string.h>
#include <msg.h>

void print(const string str, uint8_t color) {
    printstr(str, color);
}

void println(const string str, uint8_t color) {
    print(str, color);
    print("\n", color);
}

void print_error(const string str) {
    print(" [ ", 0x07);
    print("ERROR", 0x04);
    print(" ] ", 0x07);
    print(str, 0x07);
    print("\n", 0x07);
}

void print_info(const string str) {
    print(" [ ", 0x07);
    print("INFO", 0x01);
    print(" ] ", 0x07);
    print(str, 0x07);
    print("\n", 0x07);
}

void print_warn(const string str) {
    print(" [ ", 0x07);
    print("WARNING", 0x0E);
    print(" ] ", 0x07);
    print(str, 0x07);
    print("\n", 0x07);
}

void print_ok(const string str) {
    print(" [ ", 0x07);
    print("OK", 0x0B);
    print(" ] ", 0x07);
    print(str, 0x07);
    print("\n", 0x07);
}

void print_custom(const string str, uint8_t color, uint8_t scolor) {
    print(" [ ", scolor);
    print(str, color);
    print(" ] ", scolor);
}