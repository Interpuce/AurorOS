bits 32

global isr_stub_table
global irq_stub_table
global idt_load
extern isr_handler

%macro ISR_NOERR 1
global isr%1
isr%1:
    push dword 0          ; err_code = 0
    push dword %1         ; int number
    jmp isr_common
%endmacro

%macro ISR_ERR 1
global isr%1
isr%1:
    push dword %1         ; int number
    jmp isr_common
%endmacro

%macro IRQ 1
global irq%1
irq%1:
    push dword 0          ; err_code = 0
    push dword (32 + %1)  ; IRQ mapped to IDT 32+
    jmp isr_common
%endmacro

ISR_NOERR 0
ISR_NOERR 1
ISR_NOERR 2
ISR_NOERR 3
ISR_NOERR 4
ISR_NOERR 5
ISR_NOERR 6
ISR_NOERR 7
ISR_ERR   8
ISR_NOERR 9
ISR_ERR   10
ISR_ERR   11
ISR_ERR   12
ISR_ERR   13
ISR_ERR   14
ISR_NOERR 15
ISR_NOERR 16
ISR_ERR   17
ISR_NOERR 18
ISR_NOERR 19
ISR_NOERR 20
ISR_NOERR 21
ISR_NOERR 22
ISR_NOERR 23
ISR_NOERR 24
ISR_NOERR 25
ISR_NOERR 26
ISR_NOERR 27
ISR_NOERR 28
ISR_NOERR 29
ISR_ERR   30
ISR_NOERR 31

%assign n 0
%rep 16
    IRQ n
    %assign n n+1
%endrep

isr_common:
    pusha                   ; 8 regs
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10            ; kernel data segment
    mov ds, ax
    mov es, ax

    mov eax, esp            ; eax = pointer to registers_t
    push eax
    call isr_handler
    add esp, 4

    pop gs
    pop fs
    pop es
    pop ds
    popa
    add esp, 8              ; pop int_no, err_code
    iretd

section .data
isr_stub_table:
    %assign i 0
    %rep 32
        dd isr%+i
        %assign i i+1
    %endrep

irq_stub_table:
    %assign i 0
    %rep 16
        dd irq%+i
        %assign i i+1
    %endrep

section .text
idt_load:
    mov eax, [esp + 4]
    lidt [eax]
    ret
