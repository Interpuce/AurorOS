/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <types.h>
#include <system.h>

#define WIDTH  320 
#define HEIGHT 240 

#define VGA_MEMORY 0xA0000  // VGA buffer address
#define VGA_PORT_INDEX 0x3C4  // VGA port index register
#define VGA_PORT_DATA 0x3C5  // VGA port data register

// VGA-related registers for controlling the display
#define VGA_MODE_REGISTER 0x3C0  // Register for the VGA control

uint32_t* framebuffer = (uint32_t*) VGA_MEMORY;

// Function to initialize the VGA in text mode or graphics mode
void vga_init() {
    // Set VGA mode here (e.g., mode 0x13 for 320x200 256 colors)
    outb(VGA_PORT_INDEX, 0x00);  // Select mode register
    outb(VGA_PORT_DATA, 0x13);   // Set to 320x200 graphics mode
}

// Function to clear the canvas by setting all pixels to a specified color
void clear_canvas(uint32_t color) {
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            framebuffer[y * WIDTH + x] = color;
        }
    }
}

// Function to draw a single pixel at (x, y) with a specific color
void draw_pixel(int x, int y, uint32_t color) {
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        framebuffer[y * WIDTH + x] = color;
    }
}

// Function to draw a rectangle at (x, y) with the given width, height, and color
void draw_rectangle(int x, int y, int width, int height, uint32_t color) {
    for (int j = y; j < y + height; j++) {
        for (int i = x; i < x + width; i++) {
            if (i >= 0 && i < WIDTH && j >= 0 && j < HEIGHT) {
                framebuffer[j * WIDTH + i] = color;
            }
        }
    }
}

// Function to draw a horizontal line starting at (x, y) with a specific length and color
void draw_horizontal_line(int x, int y, int length, uint32_t color) {
    for (int i = x; i < x + length; i++) {
        if (i >= 0 && i < WIDTH && y >= 0 && y < HEIGHT) {
            framebuffer[y * WIDTH + i] = color;
        }
    }
}

// Function to draw a vertical line starting at (x, y) with a specific length and color
void draw_vertical_line(int x, int y, int length, uint32_t color) {
    for (int i = y; i < y + length; i++) {
        if (x >= 0 && x < WIDTH && i >= 0 && i < HEIGHT) {
            framebuffer[i * WIDTH + x] = color;
        }
    }
}