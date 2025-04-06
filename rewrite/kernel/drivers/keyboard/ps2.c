/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <asm/ports.h>
#include <drivers/screen.h>
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

static bool is_shift_left_pressed = false;
static bool is_shift_right_pressed = false;

static uint8 key_states[128] = {0};

static uint8 read_scancode() {
    return inb(0x60);
}

void get_input(string buffer, uint16 max_length, bool secret, uint8 color) {
    uint16 length = 0;

    while (true) {
        uint8 scancode = read_scancode();
        uint8 shift_released = scancode & 0x80;
        uint8 key = scancode & ~0x80;

        if (shift_released) {
            key_states[key] = 0;

            switch (key) {
                case 0x2A:
                    is_shift_left_pressed = 0;
                    break;
                case 0x36:
                    is_shift_right_pressed = 0;
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
            delete_character();
            continue;
        }

        char c;
        switch (key) {
            case 0x2A:
            case 0x36:
                if (shift_released) break;
                if (key == 0x2A) is_shift_left_pressed = 1;
                if (key == 0x36) is_shift_right_pressed = 1;
                continue;
            default:
                c = (is_shift_left_pressed || is_shift_right_pressed) ? shift_keymap[key] : keymap[key];
                break;
        }

        if (!c) {
            continue;
        }

        if (length < max_length - 1) {
            buffer[length++] = c;
            buffer[length] = '\0';

            if (secret) {
                print("*", color);
            } else {
                string str = &c;
                print(str, color);
            }
        }
    }
}