#include "screen.h"
#include "string.h"
#include "msg.h"

void printError(const char *str) {
    printstr("[  ", 0x07);
    printstr("ERROR", 0x04);
    printstr("  ] ", 0x07);
    printstr(str, 0x07);
    printstr("\n", 0x07);
}

void printInfo(const char *str) {
    printstr("[  ", 0x07);
    printstr("INFO", 0x01);
    printstr("  ] ", 0x07);
    printstr(str, 0x07);
    printstr("\n", 0x07);
}

void printInfoB(const char *str) {
    printstr("[  ", 0x07);
    printstr("INFO", 0x01);
    printstr("  ] ", 0x07);
    printstr(str, 0x07);
    printstr("\n", 0x07);
}

void printSuccess(const char *str) {
    printstr("[  ", 0x07);
    printstr("OK", 0x0B);
    printstr("  ] ", 0x07);
    printstr(str, 0x07);
    printstr("\n", 0x07);
}

void printCustom(const char *str, uint8_t color, uint8_t scolor) {
    printstr("[  ", scolor);
    printstr(str, color);
    printstr("  ] ", scolor);
}