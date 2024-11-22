/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

// DECLARATION FILE
// The source for these function can be found at:
//   drivers/vga.c

#ifndef INCLUDE_DECLARATIONS_VGA
#define INCLUDE_DECLARATIONS_VGA 1

#include <types.h>

extern void printchs(string ch, int8 clr);
extern void printch(char c, int8 color);
extern void nl_check();
extern void scroll_up(uint8 lineNumber);
extern void clear_console();
extern void update_cursor();
extern void clear_line(uint8 from,uint8 to);

#endif