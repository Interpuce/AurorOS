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
#include <fs-emulated.h>
#include <panic.h>
#include <asm/power.h>

extern int terminal_main(uint16_t theme); // from /apps/terminal/terminal.c

void main() {
    init_memory();
    init_virtual_fs();
    if (emulated_fs_root->child_count == 0) {
        kernelpanic("EMULATED_FS_BROKEN", NULL);
    }

    clearscreen();
    println("", 0x07);
    println("                        @@@@@@@@@                           ", 0x0B);                                                            
    println("                       @@@@@@@@@@@@@@                       ", 0x0B);                                                                 
    println("                       @@@@@@@@@@@@@@@@                     ", 0x0B);                                 
    println("                       @@@@@@@@@@@@@@@@                     ", 0x0B);
    println("                    @@@@@@@@@@@@@@@@@@@     @@@@@           ", 0x0B);
    println("                    @@@@@@@@@@@@@@@@@@     @@@@@@@@         ", 0x0B);
    println("                    @@@@@@@@@@@@@@@@      @@@@@@@@@@        ", 0x0B);
    println("                    @@@@@@@@@@@@@@@@     @@@@@@@@@@@        ", 0x0B);
    println("                    @@@@@@@@@@@@@@@@    @@@@@   @@@@@       ", 0x0B);
    println("               @@@@@@@@@@@@@@@@@@@@    @@@@@@   @@@@@@      ", 0x0B);
    println("               @@@@@@@@@@@@@@@@@@     @@@@@@     @@@@@@     ", 0x0B);
    println("                @@@@@@@@@@@@@@@@     @@@@@@       @@@@@     ", 0x0B);
    println("                 @@@@@@@@@@@@@@@    @@@@@@@@@@@@@@@@@@@@    ", 0x0B);
    println("                  @@@@@@@@@@@@     @@@@@@@@@@@@@@@@@@@@@@   ", 0x0B);
    println("                      @@@@@       @@@@@@@@@@@@@@@@@@@@@@@@  ", 0x0B);
    println("                                 @@@@@@             @@@@@@  ", 0x0B);
    println("                                 @@@@@@             @@@@@@  ", 0x0B);
    println("                                  @@@@               @@@@   ", 0x0B);
    println("", 0x07);
    terminal_main(0x0B);

    shutdown();
    return;
}
