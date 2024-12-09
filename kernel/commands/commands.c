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
#include <msg.h>
#include <string.h>

void cowsay(char message[1024]) {
    int message_length = strlen(message);

    print("  ", 0x07);
    for (int i = 0; i < message_length + 2; i++) {
        printchar('-', 0x07);
    }
    println("", 0x07);

    print(" < ", 0x07);
    print(message, 0x07);
    print(" > \n", 0x07);

    print("  ", 0x07);
    for (int i = 0; i < message_length + 2; i++) {
        printchar('-', 0x07);
    }
    println("", 0x07);

    print("   \\   ^__^\n", 0x07);
    print("    \\  (oo)\\_______\n", 0x07);
    print("       (__)\\       )\\/\n", 0x07);
    print("           ||---w||\n", 0x07);
    print("           ||    ||\n", 0x07);
}

void map() {
    print("\n", 0x07); 
    print("0x01", 0x01); print(" 0x00", 0x00);
    print("0x10", 0x10); print(" 0x00", 0x00);
    print("\n", 0x07); 
    print("0x02", 0x02); print(" 0x0A", 0x0A);
    print("0x20", 0x20); print(" 0xA0", 0xA0);
    print("\n", 0x07); 
    print("0x03", 0x03); print(" 0x0B", 0x0B);
    print("0x30", 0x30); print(" 0xB0", 0xB0);
    print("\n", 0x07); 
    print("0x04", 0x04); print(" 0x0C", 0x0C);
    print("0x40", 0x40); print(" 0xC0", 0xC0);
    print("\n", 0x07); 
    print("0x05", 0x05); print(" 0x0D", 0x0D);
    print("0x50", 0x50); print(" 0xD0", 0xD0);
    print("\n", 0x07); 
    print("0x06", 0x06); print(" 0x0E", 0x0E);
    print("0x60", 0x60); print(" 0xE0", 0xE0);
    print("\n", 0x07); 
    print("0x07", 0x07); print(" 0x0F", 0x0F);
    print("0x70", 0x70); print(" 0xF0", 0xF0);
    print("\n", 0x07); 
}
