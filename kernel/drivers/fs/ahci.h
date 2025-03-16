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

typedef struct {
    uint32_t CAP;          // Capabilities Register
    uint32_t GHC;          // Global Host Control Register
    uint32_t IS;           // Interrupt Status Register
    uint32_t PI;           // Port Implemented Register
    uint32_t version;      // Version Register
    uint32_t CCC;          // Command Completion Coalescing Register
    uint32_t EM_LOC;       // Emulated Mode Location
    uint32_t EM_CTL;       // Emulated Mode Control
    uint32_t reserved[12];
    uint32_t port[32];     // Port registers (one per port)
} ahci_registers_t;
