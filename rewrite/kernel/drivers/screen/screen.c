/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <types.h>
#include "vga.h"

typedef enum {
    ScreenModeUnknown,
    ScreenModeVGA,
    ScreenModeVESA
} ScreenModes;

ScreenModes current_screen_mode = ScreenModeUnknown;

ScreenModes which_screen_mode() {
    if (current_screen_mode == ScreenModeUnknown) {
        return ScreenModeVGA; // VESA is NOT supported at this time
    } 

    return current_screen_mode;
}

void print(const string* str, uint8 color) {
    if (current_screen_mode == ScreenModeUnknown) {
        current_screen_mode = which_screen_mode();
    }

    while (*str) {
        if (current_screen_mode == ScreenModeVGA) {
            vga_print_character(*str++, color);
        }
    }
}

void paint_screen(uint8 color) {
    if (current_screen_mode == ScreenModeUnknown) {
        current_screen_mode = which_screen_mode();
    }

    if (current_screen_mode == ScreenModeVGA) {
        vga_paint_screen(color);
    }
}