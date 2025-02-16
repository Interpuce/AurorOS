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
    uint8_t bus;
    uint8_t slot;
    uint16_t vendor_id;
    uint16_t device_id;
} pci_device_t;

extern pci_device_t* pci_get_devices(uint16_t* count);
extern uint16_t pci_scan_bus();