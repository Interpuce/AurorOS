/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#pragma once

#define VGA_VIDEO_MEMORY 0xB8000
#define VGA_SCREEN_WIDTH 80
#define VGA_SCREEN_HEIGHT 25

extern void vga_print_center(const string str, uint8_t color);
extern void vga_print_character(char c, uint8_t color);
extern void vga_paint_screen(uint8_t color);
extern void vga_paint_line(uint16_t line, uint8_t color);