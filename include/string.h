#ifndef STRING_H
#define STRING_H

#include "types.h"

int strlen(const char *str);
void strcpy(char *dest, const char *src);
int streql(const char *str1, const char *str2);
int splitString(char *str, char separator, char **result, int max_splits);
char *strrchr(const char *str, int c);
char *strcat(char *dest, const char *src);
void *memcpy(void *dest, const void *src, size_t n);


#endif
