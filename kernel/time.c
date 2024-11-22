/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <types.h>
#include <system.h>
#include <rtc.h>

uint8_t read_rtc_register(uint8_t reg) {
    outb(0x70, reg);
    return inb(0x71); 
}

uint8_t rtc_time_is_updating() {
    outb(0x70, 0x0A);
    return inb(0x71) & 0x80;
}

void unsafe_get_current_time(time_t* time) {
    time->seconds = read_rtc_register(0x00);
    time->minutes = read_rtc_register(0x02);
    time->hours = read_rtc_register(0x04);

    uint8_t status_b = read_rtc_register(0x0B);
    if (!(status_b & 0x04)) {
        time->seconds = (time->seconds & 0x0F) + ((time->seconds / 16) * 10);
        time->minutes = (time->minutes & 0x0F) + ((time->minutes / 16) * 10);
        time->hours = (time->hours & 0x0F) + ((time->hours / 16) * 10);
    }
}

void get_current_time(time_t* time) {
    while (rtc_time_is_updating());
    unsafe_get_current_time(time);
}