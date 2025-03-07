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

int strlen(const string str) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}

string strcpy(string dest, const string src) {
    string orig = dest;
    while ((*dest++ = *src++) != '\0');
    return orig;
}

bool streql(const string str1, const string str2) {
    while (*str1 && *str2) {
        if (*str1++ != *str2++) {
            return false;
        }
    }
    return (*str1 == '\0' && *str2 == '\0');
}

int split_str(string str, char separator, string *result, int max_splits) {
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

string strrchr(const string str, int c) {
    string last = NULL;
    while (*str) {
        if (*str == (char)c) {
            last = (string )str;
        }
        str++;
    }
    return last;
}


string strcat(string dest, const string src) {
    string start = dest;

    while (*dest) {
        dest++;
    }

    while (*src) {
        *dest++ = *src++;
    }

    *dest = '\0';
    return start;
}

bool starts_with(const string str, const string prefix) {
    while (*prefix) {
        if (*str++ != *prefix++) {
            return false;
        }
    }
    return true;
}

string strslice (string dest, const string src, size_t n) {
    string destp = dest;
    
    for (; n != 0 && *src != 0; --n) {
        *destp++ = *src++;
    }

    *destp = 0;

    return dest;
}

int strncmp(const string s1, const string s2, size_t n) {
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

string strchr(const string s, int c) {
    while (*s != '\0') {
        if (*s == (char)c) { 
            return (string )s;
        }
        s++;
    }
    if ((char)c == '\0') {
        return (string )s;
    }
    return NULL;
}