#include <types.hpp>
#include <drivers/vesa.hpp>
#include <images.hpp>

namespace Cursor {
    void draw_cursor(int layer, int cx, int cy) {
        Layer* l = VESA::get_layer(layer);
        if (!l) return;

        for (int y = 0; y < 16; y++) {
            for (int x = 0; x < 16; x++) {
                uint8_t v = cursor_bitmap[y][x];
                if (v == 0) continue;

                int px = cx + x;
                int py = cy + y;

                if (px < 0 || py < 0 ||
                    px >= (int)l->width || py >= (int)l->height)
                    continue;

                uint32_t color =
                    (v == 1) ? 0xFFFFFFFF : 
                    (v == 2) ? 0x00010101 : 0;

                l->buffer[py * l->width + px] = color;
            }
        }
    }

    void clear_cursor(int layer, int cx, int cy) {
        Layer* l = VESA::get_layer(layer);
        if (!l) return;

        for (int y = 0; y < 16; y++) {
            for (int x = 0; x < 16; x++) {
                int px = cx + x;
                int py = cy + y;

                if (px < 0 || py < 0 ||
                    px >= (int)l->width || py >= (int)l->height)
                    continue;

                l->buffer[py * l->width + px] = 0x00000000;
            }
        }
    }
}