/**
 * -------------------------------------------------------------------------
 *                                   AurorOS
 * (c) 2022-2025 Interpuce
 * 
 * You should receive AurorOS license with this source code. If not - check:
 *  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
 * -------------------------------------------------------------------------
 */

#include <memory.h>
#include <panic.h>
#include <asm/idt.h>
#include <pci.h>
#include <fs.h>
#include <exec.h>
#include <constants.h>

// extern int terminal_main(uint16_t theme);
// ^ LEGACY CODE LINE
//    for now using fat32 support to load the terminal binary

bool init_os() {
    // initializes the memory to make `malloc` and `free` functions work; they
    // will ALWAYS fail without this
    init_memory();
    // installs the interrupts and loads the Interrupt Descriptor Table
    idt_install();
    // scans for PCI devices, they are hardly ever changed without a computer
    // restart, so they probably need to be checked only during system startup;
    // the result of this function is later used in `pci_get_devices()`
    pci_scan_bus();

    return true;
}

void start_system_loader() {
    // THIS WILL NOT WORK (BUT WILL COMPILE) AND IS ONLY A DRAFT
    //   issue 1:
    // cd-rom has other filesystem than fucking fat32, so only pendrive
    // booting will be supported; this will fuck out QEMU and other emulators
    // so while this will compile, this won't work on basically any hardware
    //   issue 2:
    // current AurorOS compiler compiles into .bin without any system files,
    // while it contains binary code, it won't be recognised, so it will
    // fuck out with the kernel panic

    const FileReadResult terminal_binary = disk_read_file('boot/terminal.bin'); // for some reason it compiles to boot so have this fucking boot
    const int terminal_exit_code = start_aef_binary(terminal_binary.content, terminal_binary.bytes_read, PERMISSION_LEVEL_MAIN);
    if (terminal_exit_code != 0) {
        kernelpanic("INIT_FAILURE", NULL);
    }
}

void main() {
    const bool os_init_success = init_os();
    if (!os_init_success) {
        kernelpanic("INIT_FAILURE", NULL);
    }
    start_system_loader();
}