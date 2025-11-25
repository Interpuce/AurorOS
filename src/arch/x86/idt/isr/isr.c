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

void isr_handler_c(uint32_t vector) {
    //print_warn("Interrupt");
    if (vector < 32) {
        //clearscreen();
        //kernelpanic("CPU_EXCEPTION", NULL);
        shutdown();
        
    } else if (vector >= 32 && vector < 48) {
        // uint8_t irq = (uint8_t)(vector - 32);
        //  ^ currently unused
        pic_send_eoi(vector);
    } else {
    }
    print_warn("We're still alive");
}
