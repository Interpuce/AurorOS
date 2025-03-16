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
#include "speaker.h"

uint8_t vfrg(uint32_t frequency) {
    return frequency >= 20 && frequency <= 20000;
}

uint8_t vdur(uint32_t duration) {
    return duration > 0;
}

void speaker_handler(uint32_t frequency, uint32_t duration, bool should_wait_to_end) {
    if (!vfrg(frequency)) {
        return;
    }
    
    if (!vdur(duration)) {
        return;
    }

    uint16_t divisor = 1193180 / frequency;

    outb(SPEAKER_PIT_CONTROL_PORT, 0xB6);

    outb(SPEAKER_PIT_CHANNEL2, (uint8_t)(divisor & 0xFF));
    outb(SPEAKER_PIT_CHANNEL2, (uint8_t)((divisor >> 8) & 0xFF));

    uint8_t tmp = inb(SPEAKER_PORT);
    outb(SPEAKER_PORT, tmp | 3);

    if (should_wait_to_end) {
        for (volatile uint32_t i = 0; i < duration * 1000; ++i) {
            asm volatile ("nop");
        }
    }

    tmp = inb(SPEAKER_PORT);
    outb(SPEAKER_PORT, tmp & ~3);
}

void speaker(uint32_t frequency, uint32_t duration) {
    speaker_handler(frequency, duration, true);
}

void speaker_nowait(uint32_t frequency, uint32_t duration) {
    speaker_handler(frequency, duration, false);
}