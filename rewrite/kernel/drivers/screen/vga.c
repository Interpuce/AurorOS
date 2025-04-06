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
#include <string.h>
#include "vga.h"

static uint16* vgaVideoMemory = (uint16*)VGA_VIDEO_MEMORY;
static uint16 cursorPosition = 0;

void vgaUpdateCursor() {
    uint16 pos = cursorPosition;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8)((pos >> 8) & 0xFF));
}

void vgaScroll() {
    for (uint16 row = 0; row < VGA_SCREEN_HEIGHT - 1; row++) {
        for (uint16 col = 0; col < VGA_SCREEN_WIDTH; col++) {
            vgaVideoMemory[row * VGA_SCREEN_WIDTH + col] = vgaVideoMemory[(row + 1) * VGA_SCREEN_WIDTH + col];
        }
    }

    for (uint16 col = 0; col < VGA_SCREEN_WIDTH; col++) {
        vgaVideoMemory[(VGA_SCREEN_HEIGHT - 1) * VGA_SCREEN_WIDTH + col] = ' ' | (0x07 << 8);
    }

    cursorPosition = (VGA_SCREEN_HEIGHT - 1) * VGA_SCREEN_WIDTH;
}

void vga_print_character(char c, uint8 color) {
    if (c == '\n') {
        cursorPosition += VGA_SCREEN_WIDTH - (cursorPosition % VGA_SCREEN_WIDTH);
    } else {
        vgaVideoMemory[cursorPosition++] = (color << 8) | c;
    }

    if (cursorPosition >= VGA_SCREEN_WIDTH * VGA_SCREEN_HEIGHT) {
        vgaScroll();
    }

    vgaUpdateCursor();
}

void vgaDeleteCharacter() {
    if (cursorPosition > 0) {
        cursorPosition--;
        vgaVideoMemory[cursorPosition] = ' ' | (0x07 << 8);
        vgaUpdateCursor();
    } else {
        vgaVideoMemory[cursorPosition] = ' ' | (0x07 << 8);
    }
}

void vgaClearScreen() {
    for (uint16 i = 0; i < VGA_SCREEN_WIDTH * VGA_SCREEN_HEIGHT; i++) {
        vgaVideoMemory[i] = ' ' | (0x07 << 8);
    }
    cursorPosition = 0;
    vgaUpdateCursor();
}

void vga_paint_screen(uint8 color) {
    for (uint16 i = 0; i < VGA_SCREEN_WIDTH * VGA_SCREEN_HEIGHT; i++) {
        vgaVideoMemory[i] = ' ' | (color << 8);
    }
    cursorPosition = 0;
    vgaUpdateCursor();
}

void vgaClearLine(uint16 line) {
    if (line >= VGA_SCREEN_HEIGHT) {
        return;
    }

    for (uint16 col = 0; col < VGA_SCREEN_WIDTH; col++) {
        vgaVideoMemory[line * VGA_SCREEN_WIDTH + col] = ' ' | (0x07 << 8);
    }

    if (cursorPosition / VGA_SCREEN_WIDTH == line) {
        cursorPosition = line * VGA_SCREEN_WIDTH;
        vgaUpdateCursor();
    }
}

void vgaPaintLine(uint16 line, uint8 color) {
    if (line >= VGA_SCREEN_HEIGHT) {
        return;
    }

    for (uint16 col = 0; col < VGA_SCREEN_WIDTH; col++) {
        vgaVideoMemory[line * VGA_SCREEN_WIDTH + col] = ' ' | (color << 8);
    }

    if (cursorPosition / VGA_SCREEN_WIDTH == line) {
        cursorPosition = line * VGA_SCREEN_WIDTH;
        vgaUpdateCursor();
    }
}

void vgaPrintCenter(const string str, uint8 color) {
    int length = strlen(str);
    if (length > VGA_SCREEN_WIDTH) {
        length = VGA_SCREEN_WIDTH;
    }

    int col = (VGA_SCREEN_WIDTH - length) / 2;

    uint16 row = cursorPosition / VGA_SCREEN_WIDTH;
    cursorPosition = row * VGA_SCREEN_WIDTH + col;

    char temp[VGA_SCREEN_WIDTH + 1];
    strcpy(temp, str);

    for (int i = 0; i < length; i++) {
        vgaVideoMemory[cursorPosition++] = (color << 8) | temp[i];
    }

    vgaUpdateCursor();
}