/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <input.h>
#include <screen.h>
#include <ports.h>
#include <msg.h>
#include <string.h>

static char keymap[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z',
    'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static char shift_keymap[128] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0,
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|', 'Z',
    'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static uint8_t SHIFT_LEFT_PRESSED = 0;
static uint8_t SHIFT_RIGHT_PRESSED = 0;

static uint8_t key_states[128] = {0};

static uint8_t readScancode() {
    return inb(0x60);
}

void readStr(char *buffer, uint16_t max_length, uint8_t secret, uint8_t color) {
    uint16_t length = 0;

    while (1) {
        uint8_t scancode = readScancode();
        uint8_t SHIFT_RELEASED = scancode & 0x80;
        uint8_t key = scancode & ~0x80;

        if (SHIFT_RELEASED) {
            key_states[key] = 0;

            switch (key) {
                case 0x2A:
                    SHIFT_LEFT_PRESSED = 0;
                    break;
                case 0x36:
                    SHIFT_RIGHT_PRESSED = 0;
                    break;
            }
            continue;
        }

        if (key_states[key]) {
            continue;
        }

        key_states[key] = 1;

        if (key == 0x1C) {
            buffer[length] = '\0';
            println("", 0x07);
            break;
        }

        if (key == 0x0E && length > 0) {
            length--;
            buffer[length] = '\0';
            delchar();
            continue;
        }

        char c;
        switch (key) {
            case 0x2A:
            case 0x36:
                if (SHIFT_RELEASED) break;
                if (key == 0x2A) SHIFT_LEFT_PRESSED = 1;
                if (key == 0x36) SHIFT_RIGHT_PRESSED = 1;
                continue;
            default:
                c = (SHIFT_LEFT_PRESSED || SHIFT_RIGHT_PRESSED) ? shift_keymap[key] : keymap[key];
                break;
        }

        if (!c) {
            continue;
        }

        if (length < max_length - 1) {
            buffer[length++] = c;
            buffer[length] = '\0';

            if (secret) {
                printchar('*', color);
            } else {
                printchar(c, color);
            }
        }
    }
}

uint8_t readYN(char* text, uint8_t color) {
    while (1) {
        printstr(" ", color);
        printstr(text, color);
        char buffer[2];
        readStr(buffer, sizeof(buffer), 0, color);
        if (streql(buffer, "Y") || streql(buffer, "y")) {
            return 1;
        } else if (streql(buffer, "N") || streql(buffer, "n")) {
            return 0;
        } else {
            print_error("Incorrect option!");
        }
        printstr("\n", 0x07);
    }
}