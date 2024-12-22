/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

extern void init_threads_system(); // from /exec/exec.c
extern int terminal_main(); // from /apps/terminal/terminal.c

void main() {
    // Initialize executables
    init_threads_system();
    terminal_main(0x0B);
}
