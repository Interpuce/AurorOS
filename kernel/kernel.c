/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

extern int terminal_main();
extern void kernelpanic(const char *errcode);

void main() {
    terminal_main(0x0B);
    kernelpanic("KERNEL_MAIN_LOOP_EXITED");
}
