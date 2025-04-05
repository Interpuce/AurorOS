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

// note:
//  screen mode independent functions can be found in /kernel/drivers/screen.c
//  low lewel, actual screen mode dependent functions can be found in /kernel/drivers/[mode name].c

extern void print(const string str, uint8 color);
extern void println(const string str, uint8 color);
extern void paintScreen(uint8 color);
extern void clearScreen();
extern void deleteCharacter();