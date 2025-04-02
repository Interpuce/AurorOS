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
#include <fs/iostream.h>
#include <exec.h>
#include <constants.h>
#include <screen.h>

// extern int terminal_main(uint16_t theme);
// ^ LEGACY CODE LINE
//    for now using fat32 support to load the terminal binary

// this function is really documented at every single point, so if this is a problem
// for you, you can always pull request deleting these comments
bool init_os() {
    // clears the screen, since functions below can output to the console
    // and that can without this lead to an unexpected behavior
    clearscreen();
    // initializes the memory to make `malloc` and `free` functions work; they
    // will ALWAYS fail without this
    init_memory();
    // installs the interrupts and loads the Interrupt Descriptor Table
    idt_install();
    // scans for PCI devices, they are hardly ever changed without a computer
    // restart, so they probably need to be checked only during system startup;
    // the result of this function is later used in `pci_get_devices()`
    pci_scan_bus();
    // loads the filesystem module
    init_fs();

    const FileReadResult terminal_binary = disk_read_file("boot/terminal.bin"); // for some reason it compiles to boot so have this fucking boot
    if (!terminal_binary.success) {
        return false;
    }
    const int terminal_exit_code = start_aef_binary(terminal_binary.content, terminal_binary.bytes_read, PERMISSION_LEVEL_MAIN);
    if (terminal_exit_code != 0) {
        return false;
    }

    return true;
}

void main() {
    const bool os_init_success = init_os();
    if (!os_init_success) {
        kernelpanic("INIT_FAILURE", NULL);
    }
}