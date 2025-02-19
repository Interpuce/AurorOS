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

void kernelpanic(const char *paniccode, const char *devnote) {
    paintscreen(COLOR_3);
    println(" ", COLOR_1);
    println(" Kernel panic!", COLOR_2);
    println(" ", COLOR_1);
    print(" AurorOS has encountered a critical problem and could not continue!", COLOR_1);
    println(" ", COLOR_1);
    print(" Kernel panic code: ", COLOR_1);
    print(paniccode, COLOR_2);
    println(" ", COLOR_1);
    
/* If a developer note is provided, print it after the panic code.
        Please note that VGA resolution doesn't provide much space, so notes should be short and formatted properly. 
        Developers are encouraged to use adjacent string literals when writing devnotes. For example, every newline in devnote can be a start of next string literal, just like below:

kernelpanic("GRAPHICS_PROCESSOR_FAILURE", 
		"GPU has malfunctioned and system has crashed." 
		"\n This may be caused by recent hardware changes, " 
		"\n loose GPU power cable (if applicable to your graphics processor), " 
		"\n or by faulty/corrupted driver (if occurs after an update). " 
        "\n You can attempt to reboot after restoring last working hardware configuration " 
		"\n and making sure everything (power cable/the GPU itself) is connected correctly. " 
		"\n If none of this works, you might need to revert to earlier version of AurorOS.");
        
        This way, code is much more readable than if it was written in a single long line.*/
    
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
        asm("hlt\n");
    }
}
