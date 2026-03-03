#include <types.h>
#include <fs/filesystem.h>
#include <asm/multiboot.h>
#include <panic.h>
#include <memory.h>

multiboot_info_t* arch_x86_64_multiboot_mb;

extern void main(void);
extern void idt_init(void);

static inline int is_long_mode(void) {
    uint32_t low, high;

    __asm__ volatile (
        "rdmsr"
        : "=a"(low), "=d"(high)
        : "c"(0xC0000080)
    );

    uint64_t efer = ((uint64_t)high << 32) | low;

    return (efer & (1ULL << 10)) != 0;
}

void arch_x86_64_late_start() {
    idt_init();

    init_memory(arch_x86_64_multiboot_mb->mmap_addr, arch_x86_64_multiboot_mb->mmap_length);
    init_fs();

    if (fs_root->child_count == 0) {
        kernelpanic("FS_BROKEN", NULL);
    }

    main();
}

void arch_x86_64_set_mb(multiboot_info_t* mb) {
    arch_x86_64_multiboot_mb = mb; 
    if (!mb) {
        kernelpanic("MB_STRUCT_NOT_AVAILABLE", "GRUB2 multiboot2 header required");
    }
}
