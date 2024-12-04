/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <types.h>
#include <screen.h>
#include <string.h>

void cowsay(char message[1024]) {
    int message_length = strlen(message);

    printstr("  ", 0x07);
    for (int i = 0; i < message_length + 2; i++) {
        printchar('-', 0x07);
    }
    println("", 0x07);

    printstr(" < ", 0x07);
    printstr(message, 0x07);
    printstr(" > \n", 0x07);

    printstr("  ", 0x07);
    for (int i = 0; i < message_length + 2; i++) {
        printchar('-', 0x07);
    }
    println("", 0x07);

    printstr("   \\   ^__^\n", 0x07);
    printstr("    \\  (oo)\\_______\n", 0x07);
    printstr("       (__)\\       )\\/\n", 0x07);
    printstr("           ||----||\n", 0x07);
    printstr("           ||    ||\n", 0x07);
}

void map() {
    printstr("\n", 0x07); 
    printstr("0x01", 0x01); printstr(" 0x0A", 0x0A);
    printstr("0x10", 0x10); printstr(" 0xA0", 0xA0);
    printstr("\n", 0x07); 
    printstr("0x02", 0x02); printstr(" 0x0B", 0x0B);
    printstr("0x20", 0x20); printstr(" 0xB0", 0xB0);
    printstr("\n", 0x07); 
    printstr("0x03", 0x03); printstr(" 0x0C", 0x0C);
    printstr("0x30", 0x30); printstr(" 0xC0", 0xC0);
    printstr("\n", 0x07); 
    printstr("0x04", 0x04); printstr(" 0x0D", 0x0D);
    printstr("0x40", 0x40); printstr(" 0xD0", 0xD0);
    printstr("\n", 0x07); 
    printstr("0x05", 0x05); printstr(" 0x0E", 0x0E);
    printstr("0x50", 0x50); printstr(" 0xE0", 0xE0);
    printstr("\n", 0x07); 
    printstr("0x06", 0x06); printstr(" 0x0F", 0x0F);
    printstr("0x60", 0x60); printstr(" 0xF0", 0xF0);
    printstr("\n", 0x07); 
    printstr("0x07", 0x07);
    printstr("0x70", 0x70);
    printstr("\n", 0x07); 
}