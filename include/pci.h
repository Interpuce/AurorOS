/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <types.h>

typedef struct {
    uint8_t bus;
    uint8_t slot;
    uint8_t function;
    uint16_t vendor_id;
    uint16_t device_id;
    uint8_t class_id;
    uint8_t subclass_id;
    uint8_t prog_if;
    uint8_t header_type;
    uint32_t bar[6];
} pci_device_t;

extern uint32_t pci_config_read32(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
extern uint16_t pci_config_read16(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
extern void pci_init();
extern uint64_t pci_get_bar64(uint8_t bus, uint8_t slot, uint8_t func, uint8_t bar_index);
extern uint8_t pci_read_config8(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);