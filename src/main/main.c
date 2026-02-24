#include <memory.h>
#include <types.h>
#include <fs/fs-emulated.h>
#include <panic.h>
#include <asm/power.h>

extern int terminal_main(uint16_t theme); // from /apps/terminal/tty.cpp

void main() {
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
