OUTPUT_FORMAT(elf64-x86-64)
ENTRY(arch_x86_64_start)
SECTIONS
{
    . = 0x100000;
    .multiboot : { *(.multiboot) }
    .text : { *(.text) }
    .data : { *(.data) }
    .bss  : { *(.bss)  }
}
