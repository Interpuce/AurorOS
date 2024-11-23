/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#ifndef INCLUDE_RTC
#define INCLUDE_RTC 1

#include <types.h>

typedef struct {
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} time_t;

extern void get_current_time(time_t* time);
extern void unsafe_get_current_time(time_t* time);
extern uint8_t rtc_time_is_updating();
extern uint8_t read_rtc_register(uint8_t reg);

#endif