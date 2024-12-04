/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#pragma once

#include <types.h>

int strlen(const char *str);
void strcpy(char *dest, const char *src);
int streql(const char *str1, const char *str2);
int split_str(char *str, char separator, char **result, int max_splits);
char *strrchr(const char *str, int c);
char *strcat(char *dest, const char *src);
void *memcpy(void *dest, const void *src, size_t n);