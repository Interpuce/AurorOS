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

void int_to_hex(int n, string buffer) {
    char hex_chars[] = "0123456789ABCDEF";
    int i = 0;
    do {
        buffer[i++] = hex_chars[n % 16];
        n /= 16;
    } while (n > 0);
    buffer[i] = '\0';
    
    for (int j = 0; j < i / 2; j++) {
        char temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
    }
}

void int_to_text(long long n, string buffer) {
    int is_negative = 0;

    if (n < 0) {
        is_negative = 1;
        n = -n;
    }

    int i = 0;
    do {
        buffer[i++] = (n % 10) + '0';
        n /= 10;
    } while (n > 0);

    if (is_negative) {
        buffer[i++] = '-';
    }

    buffer[i] = '\0';

    for (int j = 0; j < i / 2; j++) {
        char temp = buffer[j];
        buffer[j] = buffer[i - j - 1];
        buffer[i - j - 1] = temp;
    }
}