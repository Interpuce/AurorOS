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
#include "rtl8139.h"
#include <msg.h>
#include "spec.h"

// from ip.c and arp.c
extern void arp_handle_packet(uint8_t *packet, uint32_t length);
extern void ip_handle_packet(uint8_t *packet, uint32_t length);

uint16_t rtl_io_base; // base card address
uint8_t tx_cur = 0;   // current TX buffer index

void rtl_reset() {
    outb(rtl_io_base + RTL_COMMAND, RTL_RESET);
    while (inb(rtl_io_base + RTL_COMMAND) & RTL_RESET);
}

void rtl_init(uint16_t io_base) {
    rtl_io_base = io_base;
    
    rtl_reset();
    
    outb(rtl_io_base + RTL_COMMAND, RTL_RX_ENABLE | RTL_TX_ENABLE);
    outw(rtl_io_base + RTL_INTR_MASK, 0x0005);
}

void rtl_irq_handler() {
    uint8_t intr_status = inb(rtl_io_base + RTL_INTR_STATUS);

    if (intr_status & RTL_INTR_RX) {
        uint8_t buffer[1518];
        uint32_t len = rtl_receive_packet(buffer, sizeof(buffer));
        
        if (len > sizeof(struct eth_header)) {
            struct eth_header *header = (struct eth_header *)buffer;
            uint16_t ethertype = header->ethertype;
            
            if (ethertype == ETH_TYPE_IP) {
                ip_handle_packet(buffer + sizeof(struct eth_header), len - sizeof(struct eth_header));
            } else if (ethertype == ETH_TYPE_ARP) {
                arp_handle_packet(buffer + sizeof(struct eth_header), len - sizeof(struct eth_header));
            }
        }
    }

    if (intr_status & RTL_INTR_TX) {
        outb(rtl_io_base + RTL_INTR_STATUS, RTL_INTR_TX);
    }

    outb(rtl_io_base + RTL_INTR_STATUS, intr_status); 
}

uint32_t rtl_receive_packet(uint8_t *buffer, uint32_t max_len) {
    uint32_t packet_length = 0;
    uint32_t rx_status = inl(rtl_io_base + RTL_RX_BUFFER);

    if (rx_status & 0x01) {
        packet_length = inl(rtl_io_base + RTL_RX_BUFFER + 4);

        if (packet_length > max_len) {
            packet_length = max_len; 
        }

        for (uint32_t i = 0; i < packet_length; i++) {
            buffer[i] = inb(rtl_io_base + RTL_RX_BUFFER + 8 + i);
        }
    }

    return packet_length;
}

void rtl_send_packet(uint8_t *data, uint32_t len) {
    if (len > 1792) return;
    
    uint32_t tx_addr = rtl_io_base + RTL_TX_BUFFER + (tx_cur * 4);
    
    for (uint32_t i = 0; i < len; i++) {
        outb(tx_addr + i, data[i]);
    }
    
    outl(rtl_io_base + RTL_TX_STATUS + (tx_cur * 4), len);
    
    tx_cur = (tx_cur + 1) % 4;
}