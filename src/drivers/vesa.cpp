#include <drivers/vesa.hpp>

uint8_t* VESA::fb = nullptr;
uint32_t VESA::width = 0;
uint32_t VESA::height = 0;
uint32_t VESA::pitch = 0;

Layer VESA::layers[MAX_LAYERS] = {};
int VESA::layer_count = 0;

void VESA::init(multiboot_info* mb) {
    fb = (uint8_t*)(uint32_t)mb->framebuffer_addr;
    width = mb->framebuffer_width;
    height = mb->framebuffer_height;
    pitch = mb->framebuffer_pitch;
}

int VESA::create_layer(uint32_t w, uint32_t h) {
    if(layer_count >= MAX_LAYERS) return -1;
    layers[layer_count].width = w;
    layers[layer_count].height = h;
    layers[layer_count].visible = true;
    layers[layer_count].buffer = new uint32_t[w*h];
    for(uint32_t i=0;i<w*h;i++) layers[layer_count].buffer[i] = 0x00000000;
    return layer_count++;
}

void VESA::remove_layer(int id) {
    if(id < 0 || id >= layer_count) return;
    delete[] layers[id].buffer;
    layers[id].buffer = nullptr;
    layers[id].visible = false;
}

Layer* VESA::get_layer(int id) {
    if(id < 0 || id >= layer_count) return nullptr;
    return &layers[id];
}

void VESA::draw_pixel(int layer_id, uint32_t x, uint32_t y, uint32_t color) {
    Layer* layer = get_layer(layer_id);
    if(!layer) return;
    if(x >= layer->width || y >= layer->height) return;
    layer->buffer[y * layer->width + x] = color;
}

void VESA::render() {
    uint32_t* dst = (uint32_t*)fb;
    uint32_t pitch32 = pitch / 4;

    for(uint32_t y = 0; y < height; y++) {
        for(uint32_t x = 0; x < width; x++) {
            uint32_t color = 0x00000000;
            for(int l=0; l<layer_count; l++) {
                Layer& layer = layers[l];
                if(!layer.visible) continue;
                if(x < layer.width && y < layer.height) {
                    uint32_t px = layer.buffer[y * layer.width + x];
                    if(px != 0) color = px; 
                }
            }
            dst[y * pitch32 + x] = color;
        }
    }
}
