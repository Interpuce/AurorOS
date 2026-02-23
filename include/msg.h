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

extern void print_error(const char* str);
extern void print_info(const char* str);
extern void print_ok(const char* str);
extern void print_warn(const char* str);
extern void print_custom(const char* str, uint8_t color, uint8_t scolor);
extern void print(const char* str, uint8_t color);
extern void println(const char* str, uint8_t color);
extern void println_limit(const char* data, size_t len, uint8_t color);
