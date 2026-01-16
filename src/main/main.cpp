#include <types.hpp>
#include <boot/multiboot_info.hpp>
#include <drivers/vesa.hpp>
#include <images.hpp>
#include <drivers/ports.hpp>
#include <structs/x86/gdt.hpp>
#include <structs/x86/idt.hpp>
#include <gui/cursor.hpp>
#include <compiler.hpp>
#include <gui/mouse.hpp>
#include <structs/gui/window.hpp>
#include <structs/gui/fonts.hpp>

ATTRIBUTE_CFUNC 
void kernel_main(uint32_t, multiboot_info* mb) {
    extern void enter_protected_mode();

    if (mb->framebuffer_bpp != 32)
    for (;;);

    gdt_init();
    idt_init();
    pic_remap();
    Mouse::mouse_init();
    asm volatile ("sti");

    VESA::init(mb);

    enter_protected_mode();
}

int cursor_layer = -8;

ATTRIBUTE_CFUNC 
void irq12_handler() {
    Mouse::mouse_handler();  
}

#define TITLEBAR_COLOR 0xFFFFFFFF
#define TITLEBAR_HEIGHT 20

void draw_char(int layer, int x, int y, char c, uint32_t color) {
    uint8_t* glyph = font8x16[(uint8_t)c]; 
    for(int row = 0; row < 16; row++) {
        for(int col = 0; col < 8; col++) {
            if (glyph[row] & (1 << (7 - col))) { 
                VESA::draw_pixel(layer, x + col, y + row, color);
            }
        }
    }
}

void draw_text(int layer, int x, int y, const char* text, uint32_t color) {
    int start_x = x;

    while (*text) {
        if (*text == '\n') {
            y += 18;
            x = start_x;
            text++;
            continue;
        }

        draw_char(layer, x, y, *text, color);
        x += 8;
        text++;
    }
}

Window create_window(int x, int y, int width, int height, uint32_t color) {
    Window win;
    win.width = width;
    win.height = height;
    win.x = x;
    win.y = y;
    win.layer = VESA::create_layer(VESA::width, VESA::height);

    for(uint32_t j = 0; j < TITLEBAR_HEIGHT; j++) {
        for(uint32_t i = 0; i < (uint32_t)width; i++) {
            VESA::draw_pixel(win.layer, x + i, y + j, TITLEBAR_COLOR);
        }
    }

    for(uint32_t j = 0; j < (uint32_t)height; j++) {
        for(uint32_t i = 0; i < (uint32_t)width; i++) {
            VESA::draw_pixel(win.layer, x + i, y + j + TITLEBAR_HEIGHT, color);
        }
    }

    draw_text(win.layer, x + 2, y + 2, "Welcome to AurorOS", 0x00010101);
    draw_text(win.layer, x + 2, y + 2 + TITLEBAR_HEIGHT, "Welcome to Auror-GUI, a branch of\nAurorOS with graphical user interface \n(GUI)", 0x00010101);

    return win;
}

ATTRIBUTE_CFUNC
void kernel_main_pm() {

    int bg_layer = VESA::create_layer(VESA::width, VESA::height);

    for(uint32_t y = 0; y < VESA::height; y++) {
        for(uint32_t x = 0; x < VESA::width; x++) {
            uint32_t i = (y * VESA::width + x) * 4;
            uint8_t b = bg_raw[i + 0];
            uint8_t g = bg_raw[i + 1];
            uint8_t r = bg_raw[i + 2];
            uint32_t color = b | (g << 8) | (r << 16);
            VESA::draw_pixel(bg_layer, x, y, color);
        }
    }

    cursor_layer = VESA::create_layer(VESA::width, VESA::height);

    Cursor::draw_cursor(cursor_layer, Mouse::mouse_x, Mouse::mouse_y);

    create_window(100, 200, 300, 150, 0xFFAAAAAA);

    VESA::render();

    int32_t prev_mouse_x = Mouse::mouse_x;
    int32_t prev_mouse_y = Mouse::mouse_y;

    while (true) {
        if (prev_mouse_x != Mouse::mouse_x || prev_mouse_y != Mouse::mouse_y) {
            Cursor::clear_cursor(cursor_layer, prev_mouse_x, prev_mouse_y);
            Cursor::draw_cursor(cursor_layer, Mouse::mouse_x, Mouse::mouse_y);
            VESA::render();

            prev_mouse_x = Mouse::mouse_x;
            prev_mouse_y = Mouse::mouse_y;
        }
        VESA::draw_pixel(bg_layer, 2, 2, 0x00010101);
        VESA::render();
        VESA::draw_pixel(bg_layer, 2, 2, 0xFFFFFFFF);
        VESA::render();
    }
}