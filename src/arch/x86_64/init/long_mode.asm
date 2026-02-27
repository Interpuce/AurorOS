bits 32

global long_mode_init
extern arch_x86_64_long_mode_entry
extern arch_x86_64_setup_paging
extern pml4

long_mode_init:
    ; pae 
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax
    
    ; paging
    call arch_x86_64_setup_paging 
    mov eax, pml4       
    mov cr3, eax 

    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    mov eax, cr0
    or eax, 0x80000001
    mov cr0, eax

    jmp 0x08:long_mode_start

bits 64
long_mode_start:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov ss, ax

    call arch_x86_64_long_mode_entry
