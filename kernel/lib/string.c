/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <string.h>
#include <types.h>
#include <memory.h>

int strlen(const char *str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

char *strcpy(char *dest, const char *src) {
    char *orig = dest;
    while ((*dest++ = *src++) != '\0');
    return orig;
}

int streql(const char *str1, const char *str2) {
    while (*str1 && *str2) {
        if (*str1++ != *str2++) {
            return 0;
        }
    }
    return (*str1 == '\0' && *str2 == '\0') ? 1 : 0;
}

int split_str(char *str, char separator, char **result, int max_splits) {
    int count = 0;

    while (*str != '\0') {
        if (count >= max_splits) {
            return -1;
        }
        result[count++] = str;

        while (*str != separator && *str != '\0') {
            str++;
        }

        if (*str == separator) {
            *str = '\0';
            str++;
        }
    }

    return count;
}

char *strrchr(const char *str, int c) {
    char *last = NULL;
    while (*str) {
        if (*str == (char)c) {
            last = (char *)str;
        }
        str++;
    }
    return last;
}


char *strcat(char *dest, const char *src) {
    char *start = dest;

    while (*dest) {
        dest++;
    }

    while (*src) {
        *dest++ = *src++;
    }

    *dest = '\0';
    return start;
}

bool starts_with(const char *str, const char *prefix) {
    while (*prefix) {
        if (*str++ != *prefix++) {
            return false;
        }
    }
    return true;
}

char* strslice (char *dest, const char *src, size_t n) {
    char *destp = dest;
    
    for (; n != 0 && *src != 0; --n) {
        *destp++ = *src++;
    }

    *destp = 0;

    return dest;
}

int strncmp(const char *s1, const char *s2, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (s1[i] != s2[i]) {
            return (unsigned char)s1[i] - (unsigned char)s2[i];
        }
        if (s1[i] == '\0') {
            return 0;
        }
    }
    return 0;
}

uint8_t toupper(uint8_t c) {
    if (c >= 'a' && c <= 'z') {
        return c - ('a' - 'A');
    }
    return c;
}

char *strchr(const char *s, int c) {
    while (*s != '\0') {
        if (*s == (char)c) { 
            return (char *)s;
        }
        s++;
    }
    if ((char)c == '\0') {
        return (char *)s;
    }
    return NULL;
}