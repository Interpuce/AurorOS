/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#pragma once

#include <types.h>

extern void printchar(char c, uint8_t color);
extern void printstr(const char* str, uint8_t color);
extern void delchar();
extern void clearscreen();
extern void paintscreen(uint8_t color);
extern void printint(uint16_t value, uint8_t color);
extern void clearline(uint16_t line);
extern void paintline(uint16_t line, uint8_t color);
extern void printct(const string str, uint8_t color);
