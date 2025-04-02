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

static uint16* video_memory = (uint16_t*)VGA_VIDEO_MEMORY;
static uint16 cursor_pos = 0;

void vga_update_cursor() {
    uint16 pos = cursor_pos;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8)((pos >> 8) & 0xFF));
}

void vga_scroll() {
    for (uint16 row = 0; row < VGA_SCREEN_HEIGHT - 1; row++) {
        for (uint16 col = 0; col < VGA_SCREEN_WIDTH; col++) {
            video_memory[row * VGA_SCREEN_WIDTH + col] = video_memory[(row + 1) * VGA_SCREEN_WIDTH + col];
        }
    }

    for (uint16 col = 0; col < VGA_SCREEN_WIDTH; col++) {
        video_memory[(VGA_SCREEN_HEIGHT - 1) * VGA_SCREEN_WIDTH + col] = ' ' | (0x07 << 8);
    }

    cursor_pos = (VGA_SCREEN_HEIGHT - 1) * VGA_SCREEN_WIDTH;
}

void vga_print_character(char c, uint8_t color) {
    if (c == '\n') {
        cursor_pos += VGA_SCREEN_WIDTH - (cursor_pos % VGA_SCREEN_WIDTH);
    } else {
        video_memory[cursor_pos++] = (color << 8) | c;
    }

    if (cursor_pos >= VGA_SCREEN_WIDTH * VGA_SCREEN_HEIGHT) {
        vga_scroll();
    }

    vga_update_cursor();
}

void vga_delete_character() {
    if (cursor_pos > 0) {
        cursor_pos--;
        video_memory[cursor_pos] = ' ' | (0x07 << 8);
        vga_update_cursor();
    } else {
        video_memory[cursor_pos] = ' ' | (0x07 << 8);
    }
}

void vga_clear_screen() {
    for (uint16 i = 0; i < VGA_SCREEN_WIDTH * VGA_SCREEN_HEIGHT; i++) {
        video_memory[i] = ' ' | (0x07 << 8);
    }
    cursor_pos = 0;
    vga_update_cursor();
}

void vga_paint_screen(uint8_t color) {
    for (uint16 i = 0; i < VGA_SCREEN_WIDTH * VGA_SCREEN_HEIGHT; i++) {
        video_memory[i] = ' ' | (color << 8);
    }
    cursor_pos = 0;
    vga_update_cursor();
}

void vga_clear_line(uint16 line) {
    if (line >= VGA_SCREEN_HEIGHT) {
        return;
    }

    for (uint16 col = 0; col < VGA_SCREEN_WIDTH; col++) {
        video_memory[line * VGA_SCREEN_WIDTH + col] = ' ' | (0x07 << 8);
    }

    if (cursor_pos / VGA_SCREEN_WIDTH == line) {
        cursor_pos = line * VGA_SCREEN_WIDTH;
        vga_update_cursor();
    }
}

void vga_paint_line(uint16_t line, uint8_t color) {
    if (line >= VGA_SCREEN_HEIGHT) {
        return;
    }

    for (uint16 col = 0; col < VGA_SCREEN_WIDTH; col++) {
        video_memory[line * VGA_SCREEN_WIDTH + col] = ' ' | (color << 8);
    }

    if (cursor_pos / VGA_SCREEN_WIDTH == line) {
        cursor_pos = line * VGA_SCREEN_WIDTH;
        vga_update_cursor();
    }
}

void vga_print_center(const string str, uint8_t color) {
    int length = strlen(str);
    if (length > VGA_SCREEN_WIDTH) {
        length = VGA_SCREEN_WIDTH;
    }

    int col = (VGA_SCREEN_WIDTH - length) / 2;

    uint16 row = cursor_pos / VGA_SCREEN_WIDTH;
    cursor_pos = row * VGA_SCREEN_WIDTH + col;

    char temp[VGA_SCREEN_WIDTH + 1];
    strcpy(temp, str);

    for (int i = 0; i < length; i++) {
        video_memory[cursor_pos++] = (color << 8) | temp[i];
    }

    vga_update_cursor();
}