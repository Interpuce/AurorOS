OUTPUT_FORMAT(elf32-i386)
ENTRY(start)
SECTIONS
 {
   . = 0x100000;
   .multiboot : { *(.multiboot) }
   .text : { *(.text) }
   .data : { *(.data) }
   .bss  : { *(.bss)  }
 }
