/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
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
#include <asm/vga-cursor.h>

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

static uint8_t read_scancode() {
    return inb(0x60);
}

void read_str(char *buffer, uint16_t max_length, uint8_t secret, uint8_t color) {
    uint16_t length = 0;
    uint16_t cursor = 0;

    uint8_t extended = 0;

    while (1) {
        uint8_t scancode = read_scancode();

        if (scancode == 0xE0) {
            extended = 1;
            continue;
        }

        if (extended) {
            extended = 0;

            switch (scancode) {
                case 0x4B: 
                    if (cursor > 0) {
                        cursor--;
                        move_cursor_left();
                    }
                    continue;

                case 0x4D:
                    if (cursor < length) {
                        cursor++;
                        move_cursor_right();
                    }
                    continue;

                case 0x48: 
                case 0x50: 
                    continue;
            }
        }

        uint8_t RELEASED = scancode & 0x80;
        uint8_t key = scancode & ~0x80;

        if (RELEASED) {
            key_states[key] = 0;
            switch (key) {
                case 0x2A: SHIFT_LEFT_PRESSED = 0; break;
                case 0x36: SHIFT_RIGHT_PRESSED = 0; break;
            }
            continue;
        }

        if (key_states[key]) continue;
        key_states[key] = 1;

        if (key == 0x1C) {
            buffer[length] = '\0';
            println("", 0x07);
            break;
        }

        if (key == 0x0E) {
            if (cursor > 0) {
                cursor--;
                length--;

                for (uint16_t i = cursor; i < length; i++)
                    buffer[i] = buffer[i + 1];
                buffer[length] = '\0';

                move_cursor_left();
                delchar();
                print(buffer + cursor, color);
                print(" ", color);
                for (uint16_t i = cursor; i <= length; i++)
                    move_cursor_left();
            }
            continue;
        }

        char c = (SHIFT_LEFT_PRESSED || SHIFT_RIGHT_PRESSED)
            ? shift_keymap[key]
            : keymap[key];

        if (!c) continue;

        if (length < max_length - 1) {
            for (uint16_t i = length; i > cursor; i--)
                buffer[i] = buffer[i - 1];
            buffer[cursor] = c;

            length++;
            cursor++;

            if (!secret) {
                printchar(c, color);
                if (cursor < length) {
                    print(buffer + cursor, color);
                    for (uint16_t i = cursor; i < length; i++)
                        move_cursor_left();
                }
            } else {
                printchar('*', color);
                if (cursor < length) {
                    for (uint16_t i = cursor; i < length; i++)
                        printchar('*', color);
                    for (uint16_t i = cursor; i < length; i++)
                        move_cursor_left();
                }
            }

            buffer[length] = '\0';
        }
    }
}

uint8_t read_yn(char* text, uint8_t color) {
    while (1) {
        print(" ", color);
        print(text, color);
        char buffer[2];
        read_str(buffer, sizeof(buffer), 0, color);
        if (streql(buffer, "Y") || streql(buffer, "y")) {
            return 1;
        } else if (streql(buffer, "N") || streql(buffer, "n")) {
            return 0;
        } else {
            print_error("Incorrect option!");
        }
        print("\n", 0x07);
    }
}
