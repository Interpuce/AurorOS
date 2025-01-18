#include <stdint.h>
#include <memory.h>
#include <hardware/cpu.h>

void cpuid(uint32_t eax, uint32_t ecx, uint32_t* regs) {
    __asm__ volatile(
        "cpuid"
        : "=a" (regs[0]), "=b" (regs[1]), "=c" (regs[2]), "=d" (regs[3])
        : "a" (eax), "c" (ecx)
    );
}

char* get_cpu_name() {
    static char cpu_name[49];
    uint32_t regs[4];

    cpuid(0x80000002, 0, regs);
    memcpy(cpu_name, regs, sizeof(regs));

    cpuid(0x80000003, 0, regs);
    memcpy(cpu_name + 16, regs, sizeof(regs));

    cpuid(0x80000004, 0, regs);
    memcpy(cpu_name + 32, regs, sizeof(regs));

    cpu_name[48] = '\0';
    return cpu_name;
}
