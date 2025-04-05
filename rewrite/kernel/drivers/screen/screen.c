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

ScreenModes currentScreenMode = ScreenModeUnknown;

ScreenModes detectScreenMode() {
    if (currentScreenMode == ScreenModeUnknown) {
        return ScreenModeVGA; // VESA is NOT supported at this time
    } 

    return currentScreenMode;
}

void print(const string* str, uint8 color) {
    if (currentScreenMode == ScreenModeUnknown) {
        currentScreenMode = detectScreenMode();
    }

    while (*str) {
        if (currentScreenMode == ScreenModeVGA) {
            vgaPrintCharacter(*str++, color);
        }
    }
}

void paintScreen(uint8 color) {
    if (currentScreenMode == ScreenModeUnknown) {
        currentScreenMode = detectScreenMode();
    }

    if (currentScreenMode == ScreenModeVGA) {
        vgaPaintScreen(color);
    }
}