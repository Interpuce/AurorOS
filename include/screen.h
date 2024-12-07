/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#pragma once

#include <types.h>

void printchar(char c, uint8_t color);
void printstr(const char *str, uint8_t color);
void delchar();
void clearscreen();
void paintscreen(uint8_t color);
void printint(uint16_t value, uint8_t color);