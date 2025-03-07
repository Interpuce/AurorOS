/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#pragma GCC optimize ("O3")

#include <sys/display.h>
#include <stdio.h>

void cowsay(char message[1024]) {
    int message_length = strlen(message);

    cprint("  ", 0x07);
    for (int i = 0; i < message_length + 2; i++) {
        cprint("-", 0x07);
    }
    cprintln("", 0x07);

    cprint(" < ", 0x07);
    cprint(message, 0x07);
    cprint(" > \n", 0x07);

    cprint("  ", 0x07);
    for (int i = 0; i < message_length + 2; i++) {
        cprint("-", 0x07);
    }
    cprintln("", 0x07);

    cprint("   \\   ^__^\n", 0x07);
    cprint("    \\  (oo)\\_______\n", 0x07);
    cprint("       (__)\\       )\\/\n", 0x07);
    cprint("           ||---w||\n", 0x07);
    cprint("           ||    ||\n", 0x07);
}


void map() {
    cprint("\n", 0x07); 
    cprint(" 0x01", 0x01); cprint(" 0x09", 0x09);
    cprint(" 0x10", 0x10); cprint(" 0x90", 0x90);
    cprint("\n", 0x07); 
    cprint(" 0x02", 0x02); cprint(" 0x0A", 0x0A);
    cprint(" 0x20", 0x20); cprint(" 0xA0", 0xA0);
    cprint("\n", 0x07); 
    cprint(" 0x03", 0x03); cprint(" 0x0B", 0x0B);
    cprint(" 0x30", 0x30); cprint(" 0xB0", 0xB0);
    cprint("\n", 0x07); 
    cprint(" 0x04", 0x04); cprint(" 0x0C", 0x0C);
    cprint(" 0x40", 0x40); cprint(" 0xC0", 0xC0);
    cprint("\n", 0x07); 
    cprint(" 0x05", 0x05); cprint(" 0x0D", 0x0D);
    cprint(" 0x50", 0x50); cprint(" 0xD0", 0xD0);
    cprint("\n", 0x07); 
    cprint(" 0x06", 0x06); cprint(" 0x0E", 0x0E);
    cprint(" 0x60", 0x60); cprint(" 0xE0", 0xE0);
    cprint("\n", 0x07); 
    cprint(" 0x07", 0x07); cprint(" 0x0F", 0x0F);
    cprint(" 0x70", 0x70); cprint(" 0xF0", 0xF0);
    cprint("\n", 0x07); 
    cprint(" 0x08", 0x08); cprint(" 0x00", 0x00);
    cprint(" 0x80", 0x80); cprint(" 0x00", 0x00);
    cprintln("\n", 0x07); 
    cprint(" ~!@#$%^&*()_+        `1234567890-= \n", 0x0F);
    cprint(" qwertyuiop[]\\        QWERTYUIOP{}| \n", 0x0F);
    cprint(" asdfghjkl;'          ASDFGHJKL:\" \n", 0x0F);
    cprint(" zxcvbnm,./           ZXCVBNM<>? \n", 0x0F);
    cprint("\n", 0x07); 
}

void eclair(const string trt) {
    if (streql(trt, "1")) {
        cprint("\n", 0x07);
        for (uint8_t i = 0; i < 128; i++) {
            cprint(" ", 0x07);
            cprint("Hello, world! ", i);
        }
        cprint("\n", 0x07);
    } else if (streql(trt, "2")) {
        print_warn("It won't work");
    } else {
        print_error("Invalid treatment argument provided!");
    }
}

void help() {
    cprintln("", 0x07);
    cprintln(" ver - displays OS version info", 0x07);
    cprintln(" print <arg1> - prints out <arg1>", 0x07);
    cprintln(" map - prints out every VGA color and every printable character in aurorOS", 0x07);
    cprintln(" cowsay <arg1> - displays ascii art of an cow saying <arg1>", 0x07);
    cprintln("", 0x07);
}
