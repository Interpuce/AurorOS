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
#include <memory.h>
#include "acpi.h"

acpi_rsdp* acpi_find_rsdp() {
    for (uint32_t addr = 0x40000000; addr < 0x80000000; addr += 16) {
        acpi_rsdp* rsdp = (acpi_rsdp*)addr;
        
        if (memcmp(rsdp->signature, ACPI_RSDP_SIGNATURE, 8) == 0) {
            return rsdp; 
        }
    }
    return NULL;
}

acpi_rsdt* acpi_find_rsdt(uint32_t rsdt_address) {
    acpi_rsdt* rsdt = (acpi_rsdt*)rsdt_address;
    
    if (memcmp(rsdt->header.signature, ACPI_RSDT_SIGNATURE, 4) == 0) {
        return rsdt;
    }
    return NULL;
}

acpi_fadt* acpi_find_fadt(uint32_t fadt_address) {
    acpi_fadt* fadt = (acpi_fadt*)fadt_address;
    
    if (memcmp(fadt->header.signature, ACPI_FADT_SIGNATURE, 4) == 0) {
        return fadt;
    }
    return NULL;
}

bool acpi_shutdown() {
    acpi_rsdp* rsdp = acpi_find_rsdp();
    if (rsdp == NULL) {
        return false;
    }

    uint32_t rsdt_address = rsdp->rsdt_address;
    
    acpi_rsdt* rsdt = acpi_find_rsdt(rsdt_address);
    if (rsdt == NULL) {
        return false;
    }

    acpi_fadt* fadt = acpi_find_fadt(rsdt->tables[0]);
    if (fadt == NULL) {
        return false;
    }

    if (fadt->model_flags & 0x01) { 
        outb(fadt->firmware_control, 0x00);
        return true;
    }

    return false;
}

bool acpi_reboot() {
    acpi_rsdp* rsdp = acpi_find_rsdp();
    if (rsdp == NULL) {
        return false;
    }

    uint32_t rsdt_address = rsdp->rsdt_address;
    
    acpi_rsdt* rsdt = acpi_find_rsdt(rsdt_address);
    if (rsdt == NULL) {
        return false;
    }

    acpi_fadt* fadt = acpi_find_fadt(rsdt->tables[0]);
    if (fadt == NULL) {
        return false; 
    }

    if (fadt->model_flags & 0x02) { 
        outb(fadt->firmware_control, 0x01);
        return true;
    }

    return false;
}