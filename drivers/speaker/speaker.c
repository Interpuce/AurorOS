#include <ports.h>
#include <screen.h>
#include <string.h>

#define SPEAKER_PORT 0x61
#define PIT_CONTROL_PORT 0x43
#define PIT_CHANNEL2 0x42

void speaker(uint32_t frequency, uint32_t duration) {
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
