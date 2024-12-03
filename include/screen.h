#ifndef SCREEN_H
#define SCREEN_H

#include <types.h>

void printchar(char c, uint8_t color);
void printstr(const char *str, uint8_t color);
void println(const char *str, uint8_t color);
void delchar();
void clearscreen();
void paintscreen(uint8_t color);
void printint(uint16_t value, uint8_t color);

#endif
