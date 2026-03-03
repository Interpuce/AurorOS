bits 32

section .multiboot
    align 4
    dd 0x1BADB002
    dd 0x00
    dd -(0x1BADB002 + 0x00)

section .text

global arch_x86_start
extern arch_x86_real_mode_entry
extern _kernel_stack_end

arch_x86_start:
    cli
    mov esp, _kernel_stack_end
    mov ebp, esp

    mov edi, eax 
    mov esi, ebx  
    push esi   
    push edi
    call arch_x86_real_mode_entry
    add esp, 8

    hlt
