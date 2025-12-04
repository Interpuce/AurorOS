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
#include <msg.h>
#include <string.h>
#include <memory.h>
#include <ports.h>
#include <pci.h>
#include <wait.h>

static uint16_t pci_device_count = 0;

uint8_t pci_read_config8(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address;
    address = (1U << 31)   
              | ((uint32_t)bus << 16)
              | ((uint32_t)slot << 11)
              | ((uint32_t)func << 8)
              | (offset & 0xFC); 

    outl(0xCF8, address);
    uint32_t data = inl(0xCFC);
    return (data >> ((offset & 3) * 8)) & 0xFF;
}

uint32_t pci_config_read32(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address = (uint32_t)((bus << 16) | (slot << 11) | (func << 8) | (offset & 0xfc) | 0x80000000);
    outl(0xCF8, address);
    return inl(0xCFC);
}

uint16_t pci_config_read16(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t v = pci_config_read32(bus, slot, func, offset);
    return (offset & 2) ? (v >> 16) & 0xFFFF : v & 0xFFFF;
}

static inline uint32_t pci_read_config32(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address = (1 << 31) | (bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC);
    outl(0xCF8, address);
    return inl(0xCFC);
}

uint64_t pci_get_bar64(uint8_t bus, uint8_t slot, uint8_t func, uint8_t bar_index) {
    if (bar_index > 5) return 0;

    uint32_t low = pci_read_config32(bus, slot, func, 0x10 + bar_index * 4);

    if ((low & 0x4) == 0x4) {
        // 64-bit BAR
        if (bar_index == 5) return 0; // cannot have high BAR after last
        uint32_t high = pci_read_config32(bus, slot, func, 0x10 + (bar_index + 1) * 4);
        return ((uint64_t)high << 32) | (low & 0xFFFFFFF0);
    } else {
        // 32-bit BAR
        return (uint64_t)(low & 0xFFFFFFF0);
    }
}

void pci_read_bars(pci_device_t* dev) {
    for (int i = 0; i < 6; i++) {
        dev->bar[i] = pci_config_read32(dev->bus, dev->slot, dev->function, 0x10 + i * 4);
    }
}

void pci_read_device(uint8_t bus, uint8_t slot, uint8_t func) {
    uint16_t vendor = pci_config_read16(bus, slot, func, 0x00);
    if (vendor == 0xFFFF) return;

    pci_device_count++; 
    
    uint16_t device = pci_config_read16(bus, slot, func, 0x02);
    uint8_t class_id = pci_config_read16(bus, slot, func, 0x0A) >> 8;
    uint8_t subclass = pci_config_read16(bus, slot, func, 0x0A) & 0xFF;
    uint8_t prog_if = pci_config_read16(bus, slot, func, 0x08) >> 8;
    uint8_t header = pci_config_read16(bus, slot, func, 0x0E) >> 8;

    pci_device_t dev;
    dev.bus = bus;
    dev.slot = slot;
    dev.function = func;
    dev.vendor_id = vendor;
    dev.device_id = device;
    dev.class_id = class_id;
    dev.subclass_id = subclass;
    dev.prog_if = prog_if;
    dev.header_type = header;

    pci_read_bars(&dev);
}

void pci_scan_bus(uint8_t bus) {
    for (uint8_t slot = 0; slot < 32; slot++) {
        uint16_t vendor = pci_config_read16(bus, slot, 0, 0x00);
        if (vendor == 0xFFFF) continue;

        uint8_t header = pci_config_read16(bus, slot, 0, 0x0E) >> 8;
        pci_read_device(bus, slot, 0);

        if (header & 0x80) {
            for (uint8_t f = 1; f < 8; f++) {
                if (pci_config_read16(bus, slot, f, 0x00) != 0xFFFF) {
                    pci_read_device(bus, slot, f);
                }
            }
        }
    }
}

void pci_scan() {
    pci_device_count = 0;
    
    for (uint16_t bus = 0; bus < 256; bus++) {
        pci_scan_bus(bus);
    }
}

static void int_to_string(uint16_t value, char* buffer, int buffer_size) {
    if (buffer_size < 2) return;
    
    int i = buffer_size - 2;
    buffer[buffer_size - 1] = '\0';
    
    if (value == 0) {
        buffer[i] = '0';
        memmove(buffer, &buffer[i], 2);
        return;
    }
    
    while (value > 0 && i >= 0) {
        buffer[i] = '0' + (value % 10);
        value /= 10;
        i--;
    }
    
    memmove(buffer, &buffer[i + 1], buffer_size - i - 2);
}

void pci_init() {
    print_info("Started scanning PCI devices");
    
    pci_scan();
    
    char count_str[10];
    int_to_string(pci_device_count, count_str, sizeof(count_str));
    
    char msg[64];
    strcpy(msg, "Found ");
    strcat(msg, count_str);
    strcat(msg, " PCI devices");
    
    print_ok(msg);
}