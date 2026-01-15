#include <types.hpp>
#include <boot/multiboot_info.hpp>
#include <drivers/vesa.hpp>
#include <images.hpp>
#include <drivers/ports.hpp>
#include <structs/x86/gdt.hpp>
#include <structs/x86/idt.hpp>
#include <gui/cursor.hpp>
#include <compiler.hpp>

int32_t mouse_x = 100;
int32_t mouse_y = 100;

uint8_t mouse_cycle = 0;
int8_t mouse_packet[3];

ATTRIBUTE_CFUNC 
void mouse_handler() {
    uint8_t status = inb(0x64);
    if ((status & 1) && (status & 0x20)) {
        uint8_t data = inb(0x60);
        
        switch(mouse_cycle) {
            case 0:
                if (!(data & 0x08)) return;
                mouse_packet[0] = data;
                mouse_cycle++;
                break;
            case 1:
                mouse_packet[1] = data;
                mouse_cycle++;
                break;
            case 2:
                mouse_packet[2] = data;
                mouse_cycle = 0;

                int32_t rel_x = mouse_packet[1];
                int32_t rel_y = mouse_packet[2];
                if (mouse_packet[0] & 0x10) rel_x -= 256;
                if (mouse_packet[0] & 0x20) rel_y -= 256;

                mouse_x += rel_x;
                mouse_y -= rel_y; 

                if (mouse_x < 0) mouse_x = 0;
                if (mouse_y < 0) mouse_y = 0;
                if (mouse_x >= (int)VESA::width) mouse_x = VESA::width - 1;
                if (mouse_y >= (int)VESA::height) mouse_y = VESA::height - 1;
                break;
        }
    }
}

void mouse_wait(uint8_t type) {
    uint32_t timeout = 100000;
    if (type == 0) {
        while (timeout--) if ((inb(0x64) & 1) == 1) return;
    } else {
        while (timeout--) if ((inb(0x64) & 2) == 0) return;
    }
}

void mouse_write(uint8_t data) {
    mouse_wait(1);
    outb(0x64, 0xD4);
    mouse_wait(1);
    outb(0x60, data);
}

uint8_t mouse_read_data() {
    mouse_wait(0);
    return inb(0x60);
}

void mouse_init() {
    mouse_wait(1);
    outb(0x64, 0xA8); // enable mouse

    mouse_wait(1);
    outb(0x64, 0x20);
    mouse_wait(0);
    uint8_t status = inb(0x60) | 2;
    mouse_wait(1);
    outb(0x64, 0x60);
    mouse_wait(1);
    outb(0x60, status);

    mouse_write(0xF6);
    mouse_read_data();

    mouse_write(0xF4);
    mouse_read_data();
}

ATTRIBUTE_CFUNC 
void kernel_main(uint32_t, multiboot_info* mb) {
    extern void enter_protected_mode();

    if (mb->framebuffer_bpp != 32)
    for (;;);

    gdt_init();
    idt_init();
    pic_remap();
    mouse_init();
    asm volatile ("sti");

    VESA::init(mb);

    enter_protected_mode();
}

int cursor_layer = -8;

ATTRIBUTE_CFUNC 
void irq12_handler() {
    mouse_handler();  
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

    Cursor::draw_cursor(cursor_layer, mouse_x, mouse_y);
    VESA::render();

    int32_t prev_mouse_x = mouse_x;
    int32_t prev_mouse_y = mouse_y;

    while (true) {
        if (prev_mouse_x != mouse_x || prev_mouse_y != mouse_y) {
            Cursor::clear_cursor(cursor_layer, prev_mouse_x, prev_mouse_y);
            Cursor::draw_cursor(cursor_layer, mouse_x, mouse_y);
            VESA::render();

            prev_mouse_x = mouse_x;
            prev_mouse_y = mouse_y;
        }
        VESA::draw_pixel(bg_layer, 2, 2, 0x00010101);
        VESA::render();
        VESA::draw_pixel(bg_layer, 2, 2, 0xFFFFFFFF);
        VESA::render();
    }
}