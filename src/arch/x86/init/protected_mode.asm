[bits 16]
global enter_protected_mode

; C++ symbol do wywołania po wejściu w PM
extern kernel_main_pm
extern gdt_ptr
extern _kernel_stack_end

enter_protected_mode:
    cli                     ; wyłącz przerwania
    ; przejście do 32-bitowego stub
    jmp pm_start16_to32

; -----------------------------
[bits 32]
pm_start16_to32:
    ; ładowanie GDT
    lgdt [gdt_ptr]

    ; włącz PE bit w CR0
    mov eax, cr0
    or eax, 1
    mov cr0, eax

    ; FAR jump, żeby zaktualizować CS
    jmp 0x08:pm_entry

; =============================

pm_entry:
    ; ustaw wszystkie segmenty danych
    mov ax, 0x10         ; kernel data selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; ustaw stack
    mov esp, _kernel_stack_end

    ; wywołanie C++ kernel_main_pm
    call kernel_main_pm

pm_hang:
    hlt
    jmp pm_hang
