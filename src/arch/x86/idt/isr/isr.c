#include <types.h>
#include <panic.h>
#include <asm/power.h>
#include "ports.h"
#include "string.h"
#include "../../include/registers.h"

const char* cpu_exception_name(uint32_t int_no) {
    static const char* exceptions[] = {
        "Divide Error",
        "Debug",
        "Non Maskable Interrupt",
        "Breakpoint",
        "Overflow",
        "BOUND Range Exceeded",
        "Invalid Opcode",
        "Device Not Available",
        "Double Fault",
        "Coprocessor Segment Overrun (reserved)",
        "Invalid TSS",
        "Segment Not Present",
        "Stack-Segment Fault",
        "General Protection Fault",
        "Page Fault",
        "Reserved",
        "x87 Floating-Point Exception",
        "Alignment Check",
        "Machine Check",
        "SIMD Floating-Point Exception",
        "Virtualization Exception",
        "Control Protection Exception",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Reserved",
        "Hypervisor Injection Exception",
        "VMM Communication Exception",
        "Security Exception",
        "Reserved"
    };

    if (int_no < 32) {
        return exceptions[int_no];
    }

    return "Unknown";
}

void isr_handler(registers_t* regs) {
    if (regs->int_no < 32) {
        kernelpanic("CPU_EXCEPTION", str_replace("Your CPU has thrown an error\n with the following name: {name}.\n\n This is a bug in AurorOS. Please report it on\n https://github.com/Interpuce/AurorOS/issues/new/choose", "{name}", cpu_exception_name(regs->int_no)));
        return;
    }

    if (regs->int_no >= 32 && regs->int_no <= 47) {
        if (regs->int_no >= 40) {
            outb(0xA0, 0x20);  // slave EOI
        }
        outb(0x20, 0x20);      // master EOI
    }
}
