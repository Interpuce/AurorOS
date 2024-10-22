OUTPUT_FORMAT("elf64-x86-64")
OUTPUT_ARCH(x86_64)
ENTRY(main)

SECTIONS
{
    . = 0x400000;

    .text : {
        *(.text)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
        *(COMMON)
    }

    .rodata : {
        *(.rodata)
    }

    /DISCARD/ : {
        *(.comment)
        *(.note.GNU-stack)
    }
}