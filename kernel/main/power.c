/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <drivers/power.h>
#include <drivers/acpi.h>
#include <msg.h>

void do_power_action(PowerActionType action) {
    switch (action) {
        case PowerShutdown:
            if (!acpi_shutdown()) {
                print_status(CodeLogTypeError, "Cannot shutdown the OS using ACPI. Please shutdown the computer manually.");
                print_status(CodeLogTypeInfo, "Disabling the interrupts and halting the processor.");

                while (true) {
                    asm volatile (
                        "cli\n"
                        "hlt\n"
                    );
                }
            }
            break;

        case PowerReboot:
            if (!acpi_reboot()) {
                print_status(CodeLogTypeError, "Cannot reboot the OS using ACPI. Please reboot the computer manually.");
                print_status(CodeLogTypeInfo, "Disabling the interrupts and halting the processor.");

                while (true) {
                    asm volatile (
                        "cli\n"
                        "hlt\n"
                    );
                }
            }
            break;
        
        default:
            print_status(CodeLogTypeWarning, "Requested unknown ACPI operation, ignoring...");
            break;
    }
}