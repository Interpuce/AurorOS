; THIS IS A PROOF OF CONCEPT
;  after the OS will boot for the first time
;  i'll try to do my best to reqrite this 

global pml4
global arch_x86_64_setup_paging

section .bss
align 4096
pml4:
    resb 4096
pdpt:
    resb 4096
pdir:
    resb 4096

section .text
bits 32

arch_x86_64_setup_paging:
    mov eax, pdpt
    or eax, 0x3  
    mov [pml4], eax
    mov eax, pdir
    or eax, 0x3     
    mov [pdpt], eax

    mov ecx, 0  
.loop:
    mov eax, 0x200000
    mul ecx
    or eax, 0x83  
    mov [pdir + ecx * 8], eax

    inc ecx
    cmp ecx, 512
    jne .loop

    ret
