#ifndef MSG_H
#define MSG_H

#include "types.h"

void printError(const char *str);
void printInfo(const char *str);
void printSuccess(const char *str);
void printCustom(const char *str, uint8_t color, uint8_t scolor);

#endif
