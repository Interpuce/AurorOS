/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#pragma once

#define COLOR_1 0x4E
#define COLOR_2 0x4F
#define COLOR_3 0x48

#include <msg.h>
#include <types.h>
#include <screen.h>

void kernelpanic(const char *paniccode) {
    paintscreen(COLOR_3);
    println(" ", COLOR_1);
    println(" Kernel panic!", COLOR_2);
    println(" ", COLOR_1);
    print(" AurorOS has encountered a critical problem and could not continue!", COLOR_1);
    println(" ", COLOR_1);
    print(" Kernel panic code: ", COLOR_1);
    print(paniccode, COLOR_2);
    println(" ", COLOR_1);

    const char *devnote = NULL;

    if (paniccode == "EXEC_ARCH_CHECK_ERROR") {
        devnote = "Executable binary check has failed unexpectedly.\n This should not be visible at any time, so please\n contact developers on GitHub to report a bug.";
    } else if (paniccode == "REPORTED_CRASH") {
        devnote = "An AurorOS binary working on the highest permissions\n reported the crash. Please investigate any\n logs of that binary or try to do\n not run it with the highest privileges.";
    }
    
    
    if (devnote != NULL) {
        println(" ", COLOR_1);
        print(" Developer note: ", COLOR_1);
        print(devnote, COLOR_2);
        println(" ", COLOR_1);
    }
    
    println(" ", COLOR_1);
    print(" System halted and will not power off automatically. \n You can turn off your device manually by holding the power button.", COLOR_1);
    
    
    while (true) {
        // zeroing general-purpose registers for sake of security - all potentially sensitive stuff shall be removed
        asm(
            "xor %%eax, %%eax\n"
            "xor %%ebx, %%ebx\n"
            "xor %%ecx, %%ecx\n"
            "xor %%edx, %%edx\n"
            "xor %%esi, %%esi\n"
            "xor %%edi, %%edi\n"
            : 
            : 
            : "%eax", "%ebx", "%ecx", "%edx", "%esi", "%edi"
        );
        asm("cli; hlt\n");
    }
}
