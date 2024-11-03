#include <screen.h>
#include <string.h>
#include <types.h>

void print_info(string msg)
{
    print("\n[ ", 0x0F);
    print("info ", 0x01);
    print(" ] ", 0x0F);
    print(msg, 0x07);
}

void print_warn(string msg)
{
    print("\n[ ", 0x0F);
    print("warning ", 0x0E);
    print(" ] ", 0x0F);
    print(msg, 0x07);
}

void print_error(string msg)
{
    print("\n[ ", 0x0F);
    print("ERROR ", 0x04);
    print(" ] ", 0x0F);
    print(msg, 0x07);
}

void print_ok(string msg)
{
    print("\n[ ", 0x0F);
    print("OK ", 0x0A);
    print(" ] ", 0x0F);
    print(msg, 0x07);
}

void println(string msg)
{
    print("\n", 0x07);
    print(msg, 0x07);
}

void print_custom(string msg, string tp, int8 msgcl, int8 tpcl, int8 brcl)
{
    print("\n[ ", brcl);
    print(tp, tpcl);
    print(" ] ", brcl);
    print(msg, msgcl);
}
