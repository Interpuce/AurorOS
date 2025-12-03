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

extern void vga_printchar(char c, uint8_t color);
extern void vga_printstr(const char *str, uint8_t color);
extern void vga_delchar();
extern void vga_clearscreen();
extern void vga_paintscreen(uint8_t color);
extern void vga_printint(uint16_t value, uint8_t color);
extern void vga_clearline(uint16_t line);
extern void vga_paintline(uint16_t line, uint8_t color);
extern void vga_printct(const char *str, uint8_t color);