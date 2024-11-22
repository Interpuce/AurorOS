/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <constants.h>
#include <panic.h>
extern void kernel_main(void); // declares the /kernel/kernel.c file

// A main function which sets up everything and loads the kernel main function.
void kernel_load() {
    // Early AUROR_BETA_STATE check
    if (AUROR_BETA_STATE < 0 || AUROR_BETA_STATE >= 3) {
        kernel_panic("INCORRECT_BETA_STATE");
        return;
    }

    // ToDo: set up interrupts
    kernel_main();
}