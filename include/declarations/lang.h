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
//   lib/lang

#ifndef INCLUDE_DECLARATIONS_LANG
#define INCLUDE_DECLARATIONS_LANG 1

#include <types.h>

// Cases
extern void to_lower_case(string str);
extern void to_upper_case(string str);
extern void capitalize(string str);
extern void capitalize_words(string str);
extern int is_lower_case(const string str);
extern int is_upper_case(const string str);
extern void toggle_case(string str);

// Length
extern int length(const string str);

// Trim
extern void trim(string str);

#endif