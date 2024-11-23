/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#ifndef GUI_DRAWING
#define GUI_DRAWING 1

extern void vga_init();
extern void clear_canvas(uint32_t color);
extern void draw_pixel(int x, int y, uint32_t color);
extern void draw_rectangle(int x, int y, int width, int height, uint32_t color);
extern void draw_horizontal_line(int x, int y, int length, uint32_t color);
extern void draw_vertical_line(int x, int y, int length, uint32_t color);

#endif