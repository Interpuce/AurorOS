/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * -------------------------------------------------------------------------
 */

#include <types.h>
#include <asm/multiboot.h>
#include <screen/fonts/font-8x8.h>
#include <screen/vesa.h>

static uint32_t* vesa_fb = NULL;
static uint32_t  vesa_fb_pitch = 0;
static uint32_t  vesa_fb_width = 0;
static uint32_t  vesa_fb_height = 0;
static uint32_t  vesa_fb_bpp = 0;

static int vesa_cursor_x = 0;
static int vesa_cursor_y = 0;

void vesa_init(multiboot_info_t* mbi) {
    vesa_fb = (uint32_t*)(uintptr_t) mbi->framebuffer_addr;
    vesa_fb_pitch = mbi->framebuffer_pitch;
    vesa_fb_width = mbi->framebuffer_width;
    vesa_fb_height = mbi->framebuffer_height;
    vesa_fb_bpp = mbi->framebuffer_bpp;
    vesa_cursor_x = 0;
    vesa_cursor_y = 0;
}

void vesa_putpixel(int x, int y, uint32_t color) {
    if (!vesa_fb) return;
    if (x < 0 || y < 0 || x >= (int)vesa_fb_width || y >= (int)vesa_fb_height) return;

    uint8_t *base = (uint8_t*)vesa_fb;
    uint32_t bytes = vesa_fb_bpp / 8;
    uint8_t *pix = base + y * vesa_fb_pitch + x * bytes;

    if (bytes >= 3) {
        pix[0] = color & 0xFF;  
        pix[1] = (color >> 8) & 0xFF; 
        pix[2] = (color >> 16) & 0xFF;  
    }
    if (bytes == 4) pix[3] = (color >> 24) & 0xFF;
}

void vesa_clear(uint32_t color) {
    for (uint32_t y = 0; y < vesa_fb_height; y++) {
        for (uint32_t x = 0; x < vesa_fb_width; x++) {
            vesa_putpixel(x, y, color);
        }
    }
    vesa_cursor_x = 0;
    vesa_cursor_y = 0;
}

void vesa_draw_char(int x, int y, char c, uint32_t fg, uint32_t bg) {
    const uint8_t* glyph = (const uint8_t*) font8x8_basic[(uint8_t)c];

    for (int row = 0; row < 8; row++) {
        uint8_t line = glyph[row];

        for (int col = 0; col < 8; col++) {
            uint32_t color = (line & (1 << (7 - col))) ? fg : bg;
            vesa_putpixel(x + col, y + row, color);
        }
    }
}

void vesa_draw_string(int x, int y, const char* str, uint32_t fg, uint32_t bg) {
    while (*str) {
        vesa_draw_char(x, y, *str, fg, bg);
        x += 8;
        str++;
    }
}

void vesa_print(const char* text) {
    while (*text) {
        char c = *text++;
        if (c == '\n') {
            vesa_cursor_x = 0;
            vesa_cursor_y += 8;
            continue;
        }
        vesa_draw_char(vesa_cursor_x, vesa_cursor_y, c, 0xFFFFFFFF, 0x00000000);
        vesa_cursor_x += 8;
        if (vesa_cursor_x + 8 >= (int)vesa_fb_width) {
            vesa_cursor_x = 0;
            vesa_cursor_y += 8;
        }
    }
}

void vesa_printchar(char c, uint32_t color) {
    vesa_draw_char(vesa_cursor_x, vesa_cursor_y, c, color, 0x00000000);
    vesa_cursor_x += 8;
    if (vesa_cursor_x + 8 >= (int)vesa_fb_width) {
        vesa_cursor_x = 0;
        vesa_cursor_y += 8;
    }
}

void vesa_printstr(const char* str, uint32_t color) {
    while (*str) {
        if (*str == '\n') {
            vesa_cursor_x = 0;
            vesa_cursor_y += 8;
            str++;
            continue;
        }
        vesa_printchar(*str++, color);
    }
}

void vesa_delchar() {
    if (vesa_cursor_x >= 8) {
        vesa_cursor_x -= 8;
    } else if (vesa_cursor_y >= 8) {
        vesa_cursor_y -= 8;
        vesa_cursor_x = (int)vesa_fb_width - 8;
    }
    vesa_draw_char(vesa_cursor_x, vesa_cursor_y, ' ', 0x00000000, 0x00000000);
}

void vesa_clearscreen() {
    vesa_clear(0x00000000);
}

void vesa_paintscreen(uint32_t color) {
    vesa_clear(color);
}

static char _vesa_intbuf[16];
void vesa_printint(uint32_t val, uint32_t color) {
    int i = 14;
    _vesa_intbuf[15] = 0;
    if (val == 0) {
        vesa_printchar('0', color);
        return;
    }
    while (val > 0 && i >= 0) {
        _vesa_intbuf[i--] = '0' + (val % 10);
        val /= 10;
    }
    vesa_printstr(&_vesa_intbuf[i + 1], color);
}

void vesa_clearline(uint32_t line) {
    int y = line * 8;
    for (int px_y = y; px_y < y + 8; px_y++) {
        for (int px_x = 0; px_x < (int)vesa_fb_width; px_x++) {
            vesa_putpixel(px_x, px_y, 0x00000000);
        }
    }
}

void vesa_paintline(uint32_t line, uint32_t color) {
    int y = line * 8;
    for (int px_y = y; px_y < y + 8; px_y++) {
        for (int px_x = 0; px_x < (int)vesa_fb_width; px_x++) {
            vesa_putpixel(px_x, px_y, color);
        }
    }
}

void vesa_printct(const char* str, uint32_t color) {
    int len = 0;
    const char* tmp = str;
    while (*tmp++) len++;
    int x = ((int)vesa_fb_width / 2) - (len * 4);
    vesa_draw_string(x, vesa_cursor_y, str, color, 0x00000000);
    vesa_cursor_y += 8;
}

void vesa_set_cursor(int x, int y) {
    vesa_cursor_x = x;
    vesa_cursor_y = y;
}

int vesa_get_cursor_x() { return vesa_cursor_x; }
int vesa_get_cursor_y() { return vesa_cursor_y; }

uint32_t vesa_get_width()  { return vesa_fb_width; }
uint32_t vesa_get_height() { return vesa_fb_height; }
uint32_t vesa_get_pitch()  { return vesa_fb_pitch; }
uint32_t vesa_get_bpp()    { return vesa_fb_bpp; }