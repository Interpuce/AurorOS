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
    DISK_PRIMARY_MASTER = 0,
    DISK_PRIMARY_SLAVE,
    DISK_SECONDARY_MASTER,
    DISK_SECONDARY_SLAVE
} disk_t;

extern int disk_read_sector(disk_t disk, uint32_t lba, uint8_t *buffer, bool is_atapi, bool is_sata); // also in iostream.c