#include <string.h>
#include <screen.h>
#include <msg.h>
#include <types.h>
#include <input.h>

int tinypad_main(uint8_t color, uint8_t barcolor) {
    clearscreen();
    paintline(0, barcolor);
    printct("Tinypad", barcolor);
    println(" ", 0x07);
    char buffer[1762];
    while (1) {
        read_str(buffer, sizeof(buffer), 0, color);
        if (streql(buffer, ".exit")) {
            clearscreen();
            break;
        }
    }
    return 0;
}