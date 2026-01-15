OUTPUT_FORMAT(elf32-i386)
ENTRY(arch_x86_start)
SECTIONS
{
    . = 0x100000;
    .multiboot : { *(.multiboot) }
    .text : { *(.text) }
    .data : { *(.data) }
    .bss  : { *(.bss)  }

    .stack :
    {
        . = ALIGN(16);
        _kernel_stack_start = .;
        . = . + 0x4000;
        _kernel_stack_end = .;
    }
}