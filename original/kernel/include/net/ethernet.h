/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#pragma once

#include <types.h>

typedef struct {
    uint8_t mac[6];
} ethernet_device_t;

extern void ethernet_init();
extern void ethernet_send(uint8_t *data, uint32_t length);
extern uint32_t ethernet_receive(uint8_t *buffer, uint32_t max_len);