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

extern int strlen(const string str);
extern string strcpy(char* dest, const char* src);
extern bool streql(const char* str1, const char* str2);
extern int split_str(string str, char separator, string *result, int max_splits);
extern string strrchr(const char* str, int c);
extern string strcat(string dest, const char* src);
extern bool starts_with(const char* str, const char* prefix);
extern string strslice(string dest, const char* src, size_t n);
extern int strncmp(const string s1, const string s2, size_t n);
extern uint8_t toupper(uint8_t c);
extern string strchr(const char* s, int c);