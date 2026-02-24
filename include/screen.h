#pragma once

#include <types.h>

void printchar(char c, uint8_t color);
void printstr(const char *str, uint8_t color);
void delchar();
void clearscreen();
void paintscreen(uint8_t color);
void printint(uint16_t value, uint8_t color);
void clearline(uint16_t line);
void paintline(uint16_t line, uint8_t color);
void printct(const char *str, uint8_t color);
