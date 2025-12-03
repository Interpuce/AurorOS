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
#include <screen/vga.h>

#include <types.h>
#include <screen.h>
#include <screen/vga.h>
#include <screen/vesa.h>

static screen_mode_t current_mode = SCREEN_MODE_VGA;

static void (*_printchar)(char, uint8_t) = vga_printchar;
static void (*_printstr)(const char*, uint8_t) = vga_printstr;
static void (*_delchar)() = vga_delchar;
static void (*_clearscreen)() = vga_clearscreen;
static void (*_paintscreen)(uint8_t) = vga_paintscreen;
static void (*_printint)(uint16_t, uint8_t) = vga_printint;
static void (*_clearline)(uint16_t) = vga_clearline;
static void (*_paintline)(uint16_t, uint8_t) = vga_paintline;
static void (*_printct)(const char*, uint8_t) = vga_printct;

screen_mode_t screen_get_mode() {
    return current_mode;
}

void vesa_printchar_u8(char c, uint8_t color) {
    vesa_printchar(c, (uint32_t)color);
}

void vesa_printstr_u8(const char* str, uint8_t color) {
    vesa_printstr(str, (uint32_t)color);
}

void vesa_paintscreen_u8(uint8_t color) {
    vesa_paintscreen((uint32_t)color);
}

void vesa_printint_u8(uint16_t val, uint8_t color) {
    vesa_printint((uint32_t)val, (uint32_t)color);
}

void vesa_clearline_u8(uint16_t line) {
    vesa_clearline((uint32_t)line);
}

void vesa_paintline_u8(uint16_t line, uint8_t color) {
    vesa_paintline((uint32_t)line, (uint32_t)color);
}

void vesa_printct_u8(const char* str, uint8_t color) {
    vesa_printct(str, (uint32_t)color);
}

multiboot_info_t* screen_mbi_info;

void screen_init(multiboot_info_t* mbi) {
    screen_mbi_info = mbi;
}

void screen_set_mode(screen_mode_t mode) {
    current_mode = mode;

    if (mode == SCREEN_MODE_VGA) {
        _printchar = vga_printchar;
        _printstr = vga_printstr;
        _delchar = vga_delchar;
        _clearscreen = vga_clearscreen;
        _paintscreen = vga_paintscreen;
        _printint = vga_printint;
        _clearline = vga_clearline;
        _paintline = vga_paintline;
        _printct = vga_printct;
    } else {
        _printchar = vesa_printchar_u8;
        _printstr = vesa_printstr_u8;
        _delchar = vesa_delchar;
        _clearscreen = vesa_clearscreen;
        _paintscreen = vesa_paintscreen_u8;
        _printint = vesa_printint_u8;
        _clearline = vesa_clearline_u8;
        _paintline = vesa_paintline_u8;
        _printct = vesa_printct_u8;
        vesa_init(screen_mbi_info);
        vesa_set_cursor(0,0);
        vesa_clearscreen();
    }
}

void printchar(char c, uint8_t color) { _printchar(c, color); }
void printstr(const char *str, uint8_t color) { _printstr(str, color); }
void delchar() { _delchar(); }
void clearscreen() { _clearscreen(); }
void paintscreen(uint8_t color) { _paintscreen(color); }
void printint(uint16_t value, uint8_t color) { _printint(value, color); }
void clearline(uint16_t line) { _clearline(line); }
void paintline(uint16_t line, uint8_t color) { _paintline(line, color); }
void printct(const char *str, uint8_t color) { _printct(str, color); }