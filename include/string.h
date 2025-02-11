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

extern int strlen(const char *str);
extern char *strcpy(char *dest, const char *src);
extern int streql(const char *str1, const char *str2);
extern int split_str(char *str, char separator, char **result, int max_splits);
extern char *strrchr(const char *str, int c);
extern char *strcat(char *dest, const char *src);
extern bool starts_with(const char *str, const char *prefix);
extern char* strslice(char *dest, const char *src, size_t n);