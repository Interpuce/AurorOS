bits 32

global arch_x86_start

extern _kernel_stack_end
extern kernel_main

section .multiboot
    align 4
    dd 0x1BADB002
    dd 0x00000007
    dd -(0x1BADB002 + 0x00000007)
    dd 0
    dd 0
    dd 0
    dd 0
    dd 0
    dd 0
    dd 1024
    dd 768
    dd 32

arch_x86_start:
    cli
    mov esp, _kernel_stack_end
    push ebx
    push eax
    call kernel_main
    hlt