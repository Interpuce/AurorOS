#pragma once

#include <types.h>

void read_str(char *buffer, uint16_t max_length, uint8_t secret, uint8_t color);
uint8_t read_yn(const char *text, uint8_t color);
