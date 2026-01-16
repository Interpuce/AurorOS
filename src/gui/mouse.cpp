#include <compiler.hpp>
#include <types.hpp>
#include <drivers/ports.hpp>
#include <drivers/vesa.hpp>

namespace Mouse {
    int32_t mouse_x = 100;
    int32_t mouse_y = 100;

    uint8_t mouse_cycle = 0;
    int8_t mouse_packet[3];

    void mouse_handler() {
        uint8_t status = inb(0x64);
        if ((status & 1)) {
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
}