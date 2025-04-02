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

#define RTL_COMMAND    0x37
#define RTL_RX_BUFFER  0x30
#define RTL_TX_BUFFER0 0x20
#define RTL_INTR_MASK  0x3C
#define RTL_INTR_STATUS 0x3E

#define RTL_RESET  0x10
#define RTL_RX_ENABLE 0x08
#define RTL_TX_ENABLE 0x04
#define RTL_INTR_RX 0x01
#define RTL_INTR_TX 0x02

#define RTL_TX_BUFFER   0x20
#define RTL_TX_STATUS   0x10

extern void rtl_init(uint16_t io_base);
extern void rtl_send_packet(uint8_t *data, uint32_t length);
extern uint32_t rtl_receive_packet(uint8_t *buffer, uint32_t max_len);

#define RTL8139_VENDOR_ID 0x10EC
#define RTL8139_DEVICE_ID 0x8139