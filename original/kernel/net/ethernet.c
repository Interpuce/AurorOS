/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <net/ethernet.h>
#include <pci.h>
#include <msg.h>
#include "rtl8139.h"

pci_device_t* find_ethernet_device(uint16_t* count) {
    pci_device_t* devices = pci_get_devices(count);
    for (uint16_t i = 0; i < *count; i++) {
        if (devices[i].vendor_id == RTL8139_VENDOR_ID && devices[i].device_id == RTL8139_DEVICE_ID) {
            return &devices[i];
        }
    }
    return NULL;
}

void ethernet_init() {
    uint16_t count;
    pci_device_t* eth_device = find_ethernet_device(&count);
    if (eth_device != NULL) {
        rtl_init((eth_device->bus << 8) | eth_device->slot);
        print_ok("Internet connection initialized.");
    } else {
        print_warn("AurorOS could not find the ethernet device to connect to it. Any Internet integrations");
        print_warn("will be disabled. Make sure you have Ethernet card (now supporting only RTL8139).");
    }
}

void ethernet_send(uint8_t *data, uint32_t length) {
    rtl_send_packet(data, length);
}

uint32_t ethernet_receive(uint8_t *buffer, uint32_t max_len) {
    return rtl_receive_packet(buffer, max_len);
}