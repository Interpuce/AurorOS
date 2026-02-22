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
#include <types.h>
#include <fs/fs-emulated.h>
#include <fs/filesystem.h>
#include <panic.h>
#include <asm/power.h>
#include <asm/multiboot.h>
#include <wait.h>

extern void main(void);

extern void gdt_install();
extern void idt_init();
extern void protected_mode_init();

static inline kbool is_protected_mode() {
    uint32_t cr0;
    __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
    return (cr0 & 1) != 0;
}

int arch_x86_multiboot_magic;
multiboot_info_t* arch_x86_multiboot_mb;

void arch_x86_real_mode_entry(uint32_t magic, multiboot_info_t* mb) {
    arch_x86_multiboot_magic = magic;
    arch_x86_multiboot_mb = mb;
    if (!arch_x86_multiboot_magic || !arch_x86_multiboot_mb) {
        kernelpanic("MB_STRUCT_NOT_AVAILABLE", "Please make sure you are using the GRUB 2 bootloader.");
    }

    gdt_install();
    protected_mode_init();
}

void arch_x86_protected_mode_entry() {
    asm volatile (
        "mov $0x10, %%ax\n"
        "mov %%ax, %%ds\n"
        "mov %%ax, %%es\n"
        "mov %%ax, %%fs\n"
        "mov %%ax, %%gs\n"
        "mov %%ax, %%ss\n"
        ::: "ax"
    );

    idt_init();

    init_memory(arch_x86_multiboot_mb->mmap_addr, arch_x86_multiboot_mb->mmap_length);
    init_fs();

    if (fs_root->child_count == 0) {
        kernelpanic("FS_BROKEN", NULL);
    }
    if (!is_protected_mode()) {
        kernelpanic("PROTECTED_MODE_REQUIRED", NULL);
    }

    asm volatile ("sti");

    main();
}
