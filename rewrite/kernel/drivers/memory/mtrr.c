/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <types.h>

#define MTRR_CAP 0xFE
#define MTRR_DEF_TYPE 0x2F
#define MTRR_PHY_BASE 0x200
#define MTRR_PHY_MASK 0x201

uint64_t read_mtrr(uint32_t mtrr_index) {
    uint64_t base = 0;
    uint64_t mask = 0;
    
    asm volatile(
        "rdmsr\n"
        : "=A"(base)
        : "c"(mtrr_index)
    );

    asm volatile(
        "rdmsr\n"
        : "=A"(mask)
        : "c"(mtrr_index + 1)
    );

    return base & mask;
}

uint64_t mttr_get_total_memory() {
    uint64_t total_memory = 0;
    uint32_t mtrr_index = 0;

    while (mtrr_index < 8) {
        uint64_t memory_range = read_mtrr(mtrr_index);
        total_memory += memory_range;
        mtrr_index++;
    }

    return total_memory;
}