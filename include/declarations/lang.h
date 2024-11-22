/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

// DECLARATION FILE
// The source for these function can be found at:
//   lib/math

#pragma once

#include <types.h>

// Cases
extern void to_lower_case(char* str);
extern void to_upper_case(char* str);
extern void capitalize(char* str);
extern void capitalize_words(char* str);
extern int is_lower_case(const char* str);
extern int is_upper_case(const char* str);
extern void toggle_case(char* str);