/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <memory.h>
#include <types.h>

extern int terminal_main(uint16_t theme); // from /apps/terminal/terminal.c

void main() {
    init_memory();
    terminal_main(0x0B);
}
