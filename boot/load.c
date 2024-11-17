/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

extern void kernel_main(void); // declares the /kernel/kernel.c file

// A main function which sets up everything and loads the kernel main function.
void kernel_load() {
    // ToDo: set up interrupts
    kernel_main();
}