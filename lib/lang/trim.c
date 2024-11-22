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

void trim(string str) {
    int start = 0;
    int end = 0;

    while (str[start] == ' ' || str[start] == '\t' || str[start] == '\n') {
        start++;
    }

    for (end = start; str[end] != '\0'; end++);
    while (end > start && (str[end - 1] == ' ' || str[end - 1] == '\t' || str[end - 1] == '\n')) {
        end--;
    }

    int i = 0;
    while (start < end) {
        str[i++] = str[start++];
    }
    str[i] = '\0';
}
