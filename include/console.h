/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <declarations/vga.h>
#include <string.h>
#include <types.h>
#include <input.h>

void print(string msg, int8 color) {
    printchs(msg, color);
}

void println(string msg, int8 color)
{
    print(msg, color);
    print("\n", 0x07);
}

void print_info(string msg)
{
    print("\n[ ", 0x0F);
    print("INFO", 0x01);
    print(" ] ", 0x0F);
    println(msg, 0x07);
}

void print_warn(string msg)
{
    print("\n[ ", 0x0F);
    print("WARNING", 0x0E);
    print(" ] ", 0x0F);
    print(msg, 0x07);
}

void print_error(string msg)
{
    print("\n[ ", 0x0F);
    print("ERROR", 0x04);
    print(" ] ", 0x0F);
    println(msg, 0x07);
}

void print_ok(string msg)
{
    print("\n[ ", 0x0F);
    print("OK", 0x0A);
    print(" ] ", 0x0F);
    println(msg, 0x07);
}

void print_custom(string msg, string tp, int8 msgcl, int8 tpcl, int8 brcl)
{
    print("\n[ ", brcl);
    print(tp, tpcl);
    print(" ] ", brcl);
    println(msg, msgcl);
}

void read(int8 color) {
    read_string(color);
}