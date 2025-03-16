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

typedef enum {
    DISK_TYPE_IDE,
    DISK_TYPE_AHCI
} disk_type_t;

typedef struct {
    disk_type_t type;
    union {
        struct {
            uint16_t base_port;
            uint8_t drive;  // 0 - master, 1 - slave
        } ide;
        struct {
            uint32_t ahci_base;
            uint8_t port;
        } ahci;
    };
    bool is_atapi;
} disk_t;

extern int disk_read_sector(disk_t *disk, uint32_t lba, uint8_t *buffer);