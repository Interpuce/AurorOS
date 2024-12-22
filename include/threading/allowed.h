/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

/**
 * This file can be really difficult to understand, but this file's main
 * goal is to provide map of allowed external functions from kernel
 * in bytecode.
 */

#include <types.h>
#include <speaker.h>

extern void print_error(const string str);
extern void print_info(const string str);
extern void print_ok(const string str);
extern void print_warn(const string str);
extern void print_custom(const string str, uint8_t color, uint8_t scolor);
extern void print(const string str, uint8_t color);
extern void println(const string str, uint8_t color);
extern void clearscreen();
extern void paintscreen(uint8_t color);
extern void *memcpy(void *dest, const void *src, size_t n);
extern void *malloc(size_t size);
extern void read_str(char *buffer, uint16_t max_length, uint8_t secret, uint8_t color);
extern uint8_t read_yn(char *text, uint8_t color);

typedef struct {
    const char *name;
    void *function;
} threading_functions_allowed;

threading_functions_allowed function_table[] = {
    {"print_error", print_error},
    {"print_info", print_info},
    {"print_ok", print_ok},
    {"print_warn", print_warn},
    {"print_custom", print_custom},
    {"print", print},
    {"println", println},
    {"clearscreen", clearscreen},
    {"paintscreen", paintscreen},
    {"memcpy", memcpy},
    {"malloc", malloc},
    {"read_str", read_str},
    {"read_yn", read_yn},
    {"speaker", speaker},
    {(const char *)NULL, NULL} // table end
};