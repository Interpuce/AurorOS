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
extern void gdt_install();
extern void idt_init();
extern void protected_mode_init();

void main() {
    gdt_install();
    protected_mode_init();
}

static inline kbool is_protected_mode()
{
    uint32_t cr0;
    __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
    return (cr0 & 1) != 0;
}

void protected_mode_entry() {
    asm volatile (
        "mov $0x10, %%ax\n"
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        "mov %%ax, %%ss\n"
        ::: "ax"
    );

    idt_init();

    init_memory();
    init_virtual_fs();

    if (emulated_fs_root->child_count == 0) {
        kernelpanic("EMULATED_FS_BROKEN", NULL);
    }
    if (!is_protected_mode()) {
        kernelpanic("PROTECTED_MODE_REQUIRED", NULL);
    }

    asm volatile ("sti");

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
