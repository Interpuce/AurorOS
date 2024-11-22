/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2024 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#ifndef INCLUDE_DECLARATIONS_TIME
#define INCLUDE_DECLARATIONS_TIME 1

#include <types.h>
#include <rtc.h>

extern void get_current_time(time_t* time);
extern void unsafe_get_current_time(time_t* time);
extern uint8_t rtc_time_is_updating();
extern uint8_t read_rtc_register(uint8_t reg);

#endif