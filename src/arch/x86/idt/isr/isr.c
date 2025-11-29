/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 *
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <types.h>
#include <panic.h>
#include <asm/power.h>
#include "isr.h"
#include "../pic/pic.h"
#include "../../include/registers.h"

void isr_handler(registers_t* regs) {
    uint32_t vector = regs->int_no;

    // CPU EXCEPTIONS (0–31)
    if (vector < 32) {
        shutdown();
        // jeśli wrócimy, to coś jest mocno nie tak
        print_warn("We're still alive after CPU exception!");
        return;
    }

    // HARDWARE IRQs (32–47)
    if (vector < 48) {
        pic_send_eoi(vector);
        return;
    }

    // Any other vectors (syscalls, IPIs, custom interrupts)
    print_warn("We're still alive");
}
