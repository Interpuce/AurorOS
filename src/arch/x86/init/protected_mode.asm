bits 32

extern arch_x86_protected_mode_entry

global protected_mode_init

protected_mode_init:
    ; enables PE bit in cr0
    ;   CR0[0] = Protection Enable
    mov eax, cr0
    or eax, 1 
    mov cr0, eax
    ; something
    mov ax, 0x18
    ltr ax
    ; far jump to the protected mode entry
    jmp 0x08:arch_x86_protected_mode_entry
