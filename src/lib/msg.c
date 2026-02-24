#include <screen.h>
#include <msg.h>

void print_error(const char* str) {
    printstr("[ ", 0x07);
    printstr("ERROR", 0x04);
    printstr(" ] ", 0x07);
    printstr(str, 0x07);
    printstr("\n", 0x07);
}

void print_info(const char* str) {
    printstr("[ ", 0x07);
    printstr("INFO", 0x01);
    printstr(" ] ", 0x07);
    printstr(str, 0x07);
    printstr("\n", 0x07);
}

void print_warn(const char* str) {
    printstr("[ ", 0x07);
    printstr("WARNING", 0x0E);
    printstr(" ] ", 0x07);
    printstr(str, 0x07);
    printstr("\n", 0x07);
}

void print_ok(const char* str) {
    printstr("[ ", 0x07);
    printstr("OK", 0x02);
    printstr(" ] ", 0x07);
    printstr(str, 0x07);
    printstr("\n", 0x07);
}

void print_custom(const char* str, uint8_t color, uint8_t scolor) {
    printstr("[ ", scolor);
    printstr(str, color);
    printstr(" ] ", scolor);
}

void print(const char* str, uint8_t color) {
    printstr(str, color);
}

void println(const char* str, uint8_t color) {
    printstr(str, color);
    printchar('\n', color);
}

void println_limit(const char* data, size_t len, uint8_t color) {
    for (size_t i = 0; i < len; i++) {
        if (data[i] == 0) break;
        printchar(data[i], color);
    }
    printchar('\n', color);
}
