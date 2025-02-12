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
#include <panic.h>
#include <asm/idt.h>

void main() {
    init_memory();
    idt_install();
    kernelpanic("NOT_IMPLEMENTED");
}