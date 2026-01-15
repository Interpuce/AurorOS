#pragma once
#include <types.hpp>
#include "boot/multiboot_info.hpp"

constexpr int MAX_LAYERS = 8;

struct Layer {
    uint32_t* buffer;
    uint32_t width;
    uint32_t height;
    bool visible;
};

class VESA {
public:
    static void init(multiboot_info* mb);

    static int create_layer(uint32_t width, uint32_t height);
    static void remove_layer(int id);
    static Layer* get_layer(int id);

    static void draw_pixel(int layer_id, uint32_t x, uint32_t y, uint32_t color);
    static void render();

    static uint8_t* fb;
    static uint32_t width;
    static uint32_t height;
    static uint32_t pitch;

private:
    static Layer layers[MAX_LAYERS];
    static int layer_count;
};
