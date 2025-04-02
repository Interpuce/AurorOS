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

#define IPV4_HEADER_SIZE 20 
#define IP_PROTOCOL_TCP 6

typedef struct {
    uint8_t version_ihl; 
    uint8_t tos; // type of service instead of terms of service btw
    uint16_t total_length;
    uint16_t identification;
    uint16_t flags_fragment_offset;
    uint8_t ttl;
    uint8_t protocol;
    uint16_t checksum;
    uint32_t src_ip;
    uint32_t dest_ip;
} ipv4_header_t;

void ip_handle_packet(uint8_t *packet, uint32_t length) {
    if (length < IPV4_HEADER_SIZE) {
        print_warn("Packet is too small to be a valid IPv4 packet.");
        return;
    }

    ipv4_header_t *ip_header = (ipv4_header_t *)packet;

    uint8_t version = ip_header->version_ihl >> 4;
    uint8_t ihl = ip_header->version_ihl & 0x0F;

    if (version != 4) {
        print_warn("Not an IPv4 packet.");
        return;
    }

    if (ihl < 5) {
        print_warn("Invalid IPv4 header length.");
        return;
    }

    uint8_t protocol = ip_header->protocol;
    if (protocol == IP_PROTOCOL_TCP) {
        print_info("Received TCP packet.");
    } else {
        print_info("Received non-TCP IPv4 packet.");
    }
}