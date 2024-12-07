#include <ports.h>
#include <screen.h>

#define VIDEO_MEMORY 0xB8000
#define SCREEN_WIDTH 81
#define SCREEN_HEIGHT 25

static uint16_t *video_memory = (uint16_t *)VIDEO_MEMORY;
static uint16_t cursor_pos = 0;

void update_cursor() {
    uint16_t pos = cursor_pos;
    outb(0x3D4, 0x0F);
    outb(0x3D5, (uint8_t)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (uint8_t)((pos >> 8) & 0xFF));
}

void scroll() {
    for (uint16_t row = 0; row < SCREEN_HEIGHT - 1; row++) {
        for (uint16_t col = 0; col < SCREEN_WIDTH; col++) {
            video_memory[row * SCREEN_WIDTH + col] = video_memory[(row + 1) * SCREEN_WIDTH + col];
        }
    }

    for (uint16_t col = 0; col < SCREEN_WIDTH; col++) {
        video_memory[(SCREEN_HEIGHT - 1) * SCREEN_WIDTH + col] = ' ' | (0x07 << 8);
    }

    cursor_pos = (SCREEN_HEIGHT - 1) * SCREEN_WIDTH;
}

void printchar(char c, uint8_t color) {
    if (c == '\n') {
        cursor_pos += SCREEN_WIDTH - (cursor_pos % SCREEN_WIDTH);
    } else {
        video_memory[cursor_pos++] = (color << 8) | c;
    }

    if (cursor_pos >= SCREEN_WIDTH * SCREEN_HEIGHT) {
        scroll();
    }

    update_cursor();
}

void printstr(const char *str, uint8_t color) {
    while (*str) {
        printchar(*str++, color);
    }
}

void printint(uint16_t value, uint8_t color) {
    char buffer[6];
    int index = 5;

    buffer[index--] = '\0';

    do {
        buffer[index--] = '0' + (value % 10);
        value /= 10;
    } while (value > 0);

    for (int i = index + 1; buffer[i] != '\0'; i++) {
        printchar(buffer[i], color);
    }
}

void println(const char *str, uint8_t color) {
    printstr(str, color);
    printchar('\n', color);
}

void delchar() {
    if (cursor_pos > 0) {
        cursor_pos--;
        video_memory[cursor_pos] = ' ' | (0x07 << 8);
        update_cursor();
    } else {
        video_memory[cursor_pos] = ' ' | (0x07 << 8);
    }
}

void clearscreen() {
    for (uint16_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        video_memory[i] = ' ' | (0x07 << 8);
    }
    cursor_pos = 0;
    update_cursor();
}

void paintscreen(uint8_t color) {
    for (uint16_t i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        video_memory[i] = ' ' | (color << 8);
    }
    cursor_pos = 0;
    update_cursor();
}
