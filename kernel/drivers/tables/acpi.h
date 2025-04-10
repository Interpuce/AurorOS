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

#define ACPI_RSDP_SIGNATURE "RSD PTR "
#define ACPI_RSDT_SIGNATURE "RSDT"
#define ACPI_XSDT_SIGNATURE "XSDT"
#define ACPI_FADT_SIGNATURE "FADT"
#define ACPI_DSDT_SIGNATURE "DSDT"

typedef struct {
    char signature[4];
    uint32_t length; 
    uint8_t revision;
    uint8_t checksum;
    uint8_t oem_id[6]; 
    uint8_t oem_table_id[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
} __attribute__((packed)) acpi_table_header;

typedef struct {
    char signature[8];
    uint8_t checksum; 
    char oem_id[6]; 
    uint8_t revision;  
    uint32_t rsdt_address;
    uint32_t length;
    uint64_t xsdt_address; 
    uint32_t checksum2; 
} __attribute__((packed)) acpi_rsdp;

typedef struct {
    acpi_table_header header;
    uint32_t tables[];
} __attribute__((packed)) acpi_rsdt;

typedef struct {
    acpi_table_header header;
    uint64_t tables[]; 
} __attribute__((packed)) acpi_xsdt;

typedef struct {
    acpi_table_header header;
    uint32_t firmware_control; 
    uint32_t dsdt; 
    uint8_t model_flags; 
    uint8_t preferred_power_state;
    uint32_t pm_profile;  
    uint32_t sci_interrupt;  
    uint8_t smi_command_port;
    uint8_t acpi_enable;
    uint8_t acpi_disable;  
    uint8_t reserved[3];
    uint32_t pm_timer; 
} __attribute__((packed)) acpi_fadt;