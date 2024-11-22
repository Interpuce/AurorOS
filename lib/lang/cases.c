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

void to_lower_case(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = str[i] + ('a' - 'A');
        }
    }
}

void to_upper_case(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] = str[i] - ('a' - 'A');
        }
    }
}

void capitalize(char* str) {
    int first_letter = 1;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            if (!first_letter) {
                str[i] = str[i] + ('a' - 'A');
            }
        } else if (str[i] >= 'a' && str[i] <= 'z') {
            if (first_letter) {
                str[i] = str[i] - ('a' - 'A');
            }
        }
        first_letter = 0;
    }
}

void capitalize_words(char* str) {
    int in_word = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            if (!in_word) {
                str[i] = str[i] - ('a' - 'A');
            }
            in_word = 1;
        } else if (str[i] >= 'A' && str[i] <= 'Z') {
            if (in_word) {
                str[i] = str[i] + ('a' - 'A');
            }
            in_word = 1;
        } else {
            in_word = 0;
        }
    }
}

int is_lower_case(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') return 0;
    }
    return 1;
}

int is_upper_case(const char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') return 0;
    }
    return 1;
}

void toggle_case(char* str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'a' && str[i] <= 'z') {
            str[i] = str[i] - ('a' - 'A');
        } else if (str[i] >= 'A' && str[i] <= 'Z') {
            str[i] = str[i] + ('a' - 'A');
        }
    }
}