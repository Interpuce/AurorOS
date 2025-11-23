; -------------------------------------------------------------------------
;                                   AurorOS
; (c) 2022-2025 Interpuce
; 
; You should receive AurorOS license with this source code. If not - check:
;  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
; -------------------------------------------------------------------------

; If you are editing this file on a Visual Studio Code, please download
; extension for x86 assembly syntax, because Visual Studio Code don't
; highlight Assembly language without additional extensions.

bits 32

global isr_common_stub
global idt_load
global isr_stub_table
global irq_stub_table

extern isr_handler_c

%macro ISR_NOERR 1
global isr%1
isr%1:
    push dword %1
    jmp isr_common_stub
%endmacro

%macro IRQ 1
global irq%1
irq%1:
    push dword (32 + %1)
    jmp isr_common_stub
%endmacro

%assign i 0
%rep 32
    ISR_NOERR i
    %assign i i+1
%endrep

%assign i 0
%rep 16
    IRQ i
    %assign i i+1
%endrep

section .data
isr_stub_table:
    dd isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7
    dd isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15
    dd isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23
    dd isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31

irq_stub_table:
    dd irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7
    dd irq8, irq9, irq10, irq11, irq12, irq13, irq14, irq15

section .text

isr_common_stub:
    pusha
    push ds
    push es
    mov ax, 0x10
    mov ds, ax
    mov es, ax

    mov eax, [esp + 36]
    push eax
    call isr_handler_c
    add esp, 4

    pop es
    pop ds
    popa
    add esp, 4
    iretd

idt_load:
    lidt [eax]
    ret
