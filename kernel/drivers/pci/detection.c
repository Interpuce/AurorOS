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
#include <ports.h>
#include <pci.h>

#define PCI_CONFIG_ADDRESS  0xCF8
#define PCI_CONFIG_DATA     0xCFC

pci_device_t pci_devices[256];
uint16_t pci_device_count = 0;

uint32_t pci_read_config(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset) {
    uint32_t address = (uint32_t)((bus << 16) | (slot << 11) | (func << 8) | (offset & 0xFC) | (1 << 31));
    outl(PCI_CONFIG_ADDRESS, address);
    return inl(PCI_CONFIG_DATA);
}

// Zmodyfikowana funkcja skanująca z pobieraniem klas
uint16_t pci_scan_bus() {
    pci_device_count = 0;
    for (uint32_t bus = 0; bus < 256; bus++) {
        for (uint8_t slot = 0; slot < 32; slot++) {
            uint32_t vendor_device = pci_read_config(bus, slot, 0, 0);
            if ((vendor_device & 0xFFFF) != 0xFFFF) {
                pci_device_t *dev = &pci_devices[pci_device_count];
                dev->bus = bus;
                dev->slot = slot;
                dev->vendor_id = vendor_device & 0xFFFF;
                dev->device_id = (vendor_device >> 16) & 0xFFFF;
                
                // Pobierz informacje o klasie
                uint32_t class_reg = pci_read_config(bus, slot, 0, 0x08);
                dev->class = (class_reg >> 24) & 0xFF;
                dev->subclass = (class_reg >> 16) & 0xFF;
                
                pci_device_count++;
            }
        }
    }
    return pci_device_count;
}

pci_dev_t pci_find_class(uint8_t class, uint8_t subclass) {
    uint16_t count;
    pci_device_t *devs = pci_get_devices(&count);
    
    for (uint16_t i = 0; i < count; i++) {
        if (devs[i].class == class && devs[i].subclass == subclass) {
            return devs[i];
        }
    }
    
    pci_dev_t invalid = { .vendor_id = 0xFFFF };
    return invalid;
}

pci_device_t* pci_get_devices(uint16_t* count) {
    *count = pci_device_count;
    return pci_devices;
}