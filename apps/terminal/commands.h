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
#include <screen.h>
#include <string.h>

void cowsay(char message[1024]);
void map();
void help();
extern int tinypad_main(uint8_t color, uint8_t barcolor);