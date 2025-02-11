/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <ports.h>
#include <screen.h>
#include <string.h>

#define SPEAKER_PORT 0x61
#define PIT_CONTROL_PORT 0x43
#define PIT_CHANNEL2 0x42

uint8_t vfrg(uint32_t frequency) {
    return frequency >= 20 && frequency <= 20000;
}

uint8_t vdur(uint32_t duration) {
    return duration > 0;
}

void speaker(uint32_t frequency, uint32_t duration) {
    if (!vfrg(frequency)) {
        return;
    }
    
    if (!vdur(duration)) {
        return;
    }

    uint16_t divisor = 1193180 / frequency;

    outb(PIT_CONTROL_PORT, 0xB6);

    outb(PIT_CHANNEL2, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL2, (uint8_t)((divisor >> 8) & 0xFF));

    uint8_t tmp = inb(SPEAKER_PORT);
    outb(SPEAKER_PORT, tmp | 3);

    for (volatile uint32_t i = 0; i < duration * 1000; ++i) {
        __asm__ volatile ("nop");
    }

    tmp = inb(SPEAKER_PORT);
    outb(SPEAKER_PORT, tmp & ~3);
}
