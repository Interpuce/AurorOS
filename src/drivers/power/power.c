#include <types.h>
#include <panic.h>

extern void internal_qemu_reboot();   // ./qemu.asm
extern void internal_qemu_shutdown(); // ./qemu.asm

void shutdown() {
    internal_qemu_shutdown();
    kernelpanic("SHUTDOWN_NOT_SUCCESSFULL", NULL);
}

void reboot() {
    internal_qemu_reboot();
    kernelpanic("REBOOT_NOT_SUCCESSFULL", NULL);
}
