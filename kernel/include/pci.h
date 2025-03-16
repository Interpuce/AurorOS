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
    uint32_t bus;
    uint8_t slot;
    uint16_t vendor_id;
    uint16_t device_id;
    uint8_t class;
    uint8_t subclass;
} pci_device_t;

typedef pci_device_t pci_dev_t;

extern pci_device_t* pci_get_devices(uint16_t* count);
extern uint16_t pci_scan_bus();
extern uint32_t pci_read(pci_dev_t dev, uint8_t offset);
extern pci_dev_t pci_find_class(uint8_t class_code, uint8_t subclass);