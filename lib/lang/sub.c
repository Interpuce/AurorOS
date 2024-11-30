/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <types.h>

bool string_starts_with(string str, string prefix) {
    if (!str || !prefix) return false;

    while (*prefix) {
        if (*str != *prefix) return false;
        str++;
        prefix++;
    }

    return true;
}