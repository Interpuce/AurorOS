OUTPUT_FORMAT(elf64-x86-64)
ENTRY(arch_x86_64_start)

SECTIONS
{
    . = 0x100000;

    .multiboot : ALIGN(4K)
    {
        *(.multiboot)
    }

    .text : ALIGN(4K)
    {
        *(.text)
    }

    .data : ALIGN(4K)
    {
        *(.data)
    }

    .bss : ALIGN(4K)
    {
        *(.common)
        *(.bss)
    }

    .stack : ALIGN(16)
    {
        _kernel_stack_start = .;
        . = . + 0x4000;
        _kernel_stack_end = .;
    }

    _kernel_end = .;
}
