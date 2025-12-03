#pragma once
#include <types.h>

#define FIS_TYPE_REG_H2D 0x27

#define HBA_GHC_AE  (1 << 31)
#define HBA_GHC_HR  (1 << 0) /* HBA reset */

#define AHCI_MAX_PORTS 32
#define AHCI_MAX_CMD_SLOTS 32
#define AHCI_CMD_TBL_SIZE 256
#define AHCI_PRDT_MAX 65536

#define HBA_PxCMD_ST  (1 << 0)  /* Start */
#define HBA_PxCMD_SUD (1 << 1)  /* Spin Up Device */
#define HBA_PxCMD_POD (1 << 2)  /* Power On Device */
#define HBA_PxCMD_CLO (1 << 3)  /* Command List Override */
#define HBA_PxCMD_FRE (1 << 4)  /* FIS Receive Enable */
#define HBA_PxCMD_FR  (1 << 14) /* FIS Receive Running */
#define HBA_PxCMD_CR  (1 << 15) /* Command List Running */

typedef volatile struct HBA_PORT {
    uint32_t clb;       // 0x00, command list base address, 1K-byte aligned
    uint32_t clbu;      // 0x04, command list base address upper 32 bits
    uint32_t fb;        // 0x08, FIS base address, 256-byte aligned
    uint32_t fbu;       // 0x0C, FIS base address upper 32 bits
    uint32_t is;        // 0x10, interrupt status
    uint32_t ie;        // 0x14, interrupt enable
    uint32_t cmd;       // 0x18, command and status
    uint32_t rsv0;      // 0x1C, reserved
    uint32_t tfd;       // 0x20, task file data
    uint32_t sig;       // 0x24, signature
    uint32_t ssts;      // 0x28, SATA status (SCR0:SStatus)
    uint32_t sctl;      // 0x2C, SATA control (SCR2:SControl)
    uint32_t serr;      // 0x30, SATA error (SCR1:SError)
    uint32_t sact;      // 0x34, SATA active (SCR3:SActive)
    uint32_t ci;        // 0x38, command issue
    uint32_t sntf;      // 0x3C, SATA notification (SCR4:SNotification)
    uint32_t fbs;       // 0x40, FIS-based switch control
    uint32_t rsv1[11];  // 0x44 ~ 0x6F, reserved
    uint32_t vendor[4]; // 0x70 ~ 0x7F, vendor specific
} HBA_PORT;

typedef volatile struct HBA_MEM {
    uint32_t cap;        // 0x00, Host capability
    uint32_t ghc;        // 0x04, Global host control
    uint32_t is;         // 0x08, Interrupt status
    uint32_t pi;         // 0x0C, Port implemented
    uint32_t vs;         // 0x10, Version
    uint32_t ccc_ctl;    // 0x14, Command completion coalescing control
    uint32_t ccc_pts;    // 0x18, Command completion coalescing ports
    uint32_t em_loc;     // 0x1C, Enclosure management location
    uint32_t em_ctl;     // 0x20, Enclosure management control
    uint32_t cap2;       // 0x24, Host capabilities extended
    uint32_t bohc;       // 0x28, BIOS/OS handoff
    uint8_t  rsv[0xA0 - 0x2C]; // 0x2C ~ 0x9F reserved (116 bytes)
    uint8_t  vendor[0x100 - 0xA0]; // 0xA0 ~ 0xFF vendor specific (96 bytes)

    HBA_PORT ports[32];  // 0x100 ~, one port is 0x80 bytes, so 32 * 0x80 = 4096 bytes
} HBA_MEM;

typedef struct HBA_FIS {
    uint8_t dsfis[0x1C];    // DMA Setup FIS, 0x00 ~ 0x1B
    uint8_t pad0[0x04];     // 0x1C ~ 0x1F

    uint8_t psfis[0x14];    // PIO Setup FIS, 0x20 ~ 0x33
    uint8_t pad1[0x0C];     // 0x34 ~ 0x3F

    uint8_t rfis[0x14];     // Register – Device to Host FIS, 0x40 ~ 0x53
    uint8_t pad2[0x0C];     // 0x54 ~ 0x5F

    uint8_t sdbfis[0x08];   // Set Device Bits FIS, 0x60 ~ 0x67
    uint8_t ufis[0x40];     // Unknown FIS, 0x68 ~ 0xA7
    uint8_t rsv[0x60];      // 0xA8 ~ 0x107
} HBA_FIS;

typedef struct HBA_CMD_HEADER {
    // DW0
    uint8_t  cfl;   // Command FIS length in DWORDS, 2:0 = length
    uint8_t  a:1;   // ATAPI
    uint8_t  w:1;   // Write (1=host->device)
    uint8_t  p:1;   // Prefetchable
    uint8_t  r:1;   // Reset
    uint8_t  b:1;   // BIST
    uint8_t  c:1;   // Clear Busy upon R_OK
    uint8_t  rsv0:1;
    uint8_t  pmp:4; // Port multiplier port
    uint16_t prdtl; // Physical region descriptor table length

    // DW1
    volatile uint32_t prdbc; // PRD byte count transferred

    // DW2 & DW3
    uint32_t ctba;  // Command table descriptor base address
    uint32_t ctbau; // Upper 32 bits of CTBA

    // DW4 - DW7
    uint32_t rsv1[4];
} HBA_CMD_HEADER;

typedef struct HBA_PRDT_ENTRY {
    uint32_t dba;   // Data base address
    uint32_t dbau;  // Data base address upper 32 bits
    uint32_t rsv0;  // Reserved

    uint32_t dbc:22; // Byte count, 0-based (value = n-1)
    uint32_t rsv1:9;
    uint32_t i:1;    // Interrupt on completion
} HBA_PRDT_ENTRY;

typedef struct HBA_CMD_TBL {
    uint8_t cfis[64];    // Command FIS
    uint8_t acmd[16];    // ATAPI command
    uint8_t rsv[48];     // Reserved
    HBA_PRDT_ENTRY prdt_entry[1]; // First PRDT entry — real number = header.prdtl
} HBA_CMD_TBL;

typedef struct {
    HBA_MEM *hba;   
    uint64_t hba_phys;   
    int irq;
    int port_count;
    uint32_t ports_implemented;
} ahci_controller_t;

typedef struct {
    HBA_PORT *port;
    int port_no;
    uint64_t clb_phys;  /* command list base phys */
    void *clb_virt;
    uint64_t fb_phys;   /* FIS base phys */
    void *fb_virt;
    uint64_t cmd_tbl_phys[AHCI_MAX_CMD_SLOTS];
    void *cmd_tbl_virt[AHCI_MAX_CMD_SLOTS];
} ahci_port_t;

extern void ahci_init();