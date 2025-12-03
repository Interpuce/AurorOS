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
#include <types.h>
#include <asm/multiboot.h>

void vesa_init(multiboot_info_t* mbi);

void vesa_putpixel(int x, int y, uint32_t color);
void vesa_clear(uint32_t color);
void vesa_draw_char(int x, int y, char c, uint32_t fg, uint32_t bg);
void vesa_draw_string(int x, int y, const char* str, uint32_t fg, uint32_t bg);
void vesa_print(const char* text);

void vesa_printchar(char c, uint32_t color);
void vesa_printstr(const char* str, uint32_t color);
void vesa_delchar();
void vesa_clearscreen();
void vesa_paintscreen(uint32_t color);
void vesa_printint(uint32_t val, uint32_t color);
void vesa_clearline(uint32_t line);
void vesa_paintline(uint32_t line, uint32_t color);
void vesa_printct(const char* str, uint32_t color);

void vesa_set_cursor(int x, int y);
int  vesa_get_cursor_x();
int  vesa_get_cursor_y();

uint32_t vesa_get_width();
uint32_t vesa_get_height();
uint32_t vesa_get_pitch();
uint32_t vesa_get_bpp();