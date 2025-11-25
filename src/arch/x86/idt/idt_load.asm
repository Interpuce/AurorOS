; -------------------------------------------------------------------------
;                                   AurorOS
; (c) 2022-2025 Interpuce
;
; You should receive AurorOS license with this source code. If not - check:
;  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
; -------------------------------------------------------------------------

bits 32

global isr_common_stub
global idt_load
global isr_stub_table
global irq_stub_table

extern isr_handler_c

; -------------------------------------------------------
; ISR without error code
; -------------------------------------------------------
%macro ISR_NOERR 1
global isr%1
isr%1:
    push dword %1
    jmp isr_common_stub_err
%endmacro

; -------------------------------------------------------
; ISR with error code
; -------------------------------------------------------
%macro ISR_ERR 1
global isr%1
isr%1:
    push dword %1
    push dword 0  
    jmp isr_common_stub_err
%endmacro

; -------------------------------------------------------
; IRQs
; -------------------------------------------------------
%macro IRQ 1
global irq%1
irq%1:
    push dword (32 + %1)
    jmp isr_common_stub_noerr
%endmacro

; -------------------------------------------------------
; Generate ISR stubs (0-31)
; -------------------------------------------------------
ISR_NOERR 0    ; Divide by Zero
ISR_NOERR 1    ; Debug
ISR_NOERR 2    ; Non-Maskable Interrupt
ISR_NOERR 3    ; Breakpoint
ISR_NOERR 4    ; Overflow
ISR_NOERR 5    ; Bound Range Exceeded
ISR_NOERR 6    ; Invalid Opcode
ISR_NOERR 7    ; Device Not Available
ISR_ERR   8    ; Double Fault
ISR_NOERR 9    ; Coprocessor Segment Overrun
ISR_ERR   10   ; Invalid TSS
ISR_ERR   11   ; Segment Not Present
ISR_ERR   12   ; Stack-Segment Fault
ISR_ERR   13   ; General Protection Fault
ISR_ERR   14   ; Page Fault
ISR_NOERR 15   ; Reserved
ISR_NOERR 16   ; x87 Floating-Point Exception
ISR_ERR   17   ; Alignment Check
ISR_NOERR 18   ; Machine Check
ISR_NOERR 19   ; SIMD Floating-Point Exception
ISR_NOERR 20   ; Virtualization Exception
ISR_NOERR 21   ; Control Protection Exception
ISR_NOERR 22   ; Reserved
ISR_NOERR 23   ; Reserved
ISR_NOERR 24   ; Reserved
ISR_NOERR 25   ; Reserved
ISR_NOERR 26   ; Reserved
ISR_NOERR 27   ; Reserved
ISR_NOERR 28   ; Hypervisor Injection Exception
ISR_NOERR 29   ; VMM Communication Exception
ISR_ERR   30   ; Security Exception
ISR_NOERR 31   ; Reserved

; -------------------------------------------------------
; Generate IRQ stubs (0-15)
; -------------------------------------------------------
%assign irq_num 0
%rep 16
    IRQ irq_num
    %assign irq_num irq_num+1
%endrep

; -------------------------------------------------------
; Stub tables
; -------------------------------------------------------
section .data
isr_stub_table:
    dd isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7
    dd isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15
    dd isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23
    dd isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31

irq_stub_table:
    dd irq0, irq1, irq2, irq3, irq4, irq5, irq6, irq7
    dd irq8, irq9, irq10, irq11, irq12, irq13, irq14, irq15

; -------------------------------------------------------
; ISR Common Stub
; -------------------------------------------------------
section .text

; ISR without error code
isr_common_stub_noerr:
    pusha
    push ds
    push es
    mov ax, 0x10
    mov ds, ax
    mov es, ax

    mov eax, [esp + 40] 
    push eax
    call isr_handler_c
    add esp, 4

    pop es
    pop ds
    popa
    iretd

; ISR with error code
isr_common_stub_err:
    pusha
    push ds
    push es
    mov ax, 0x10
    mov ds, ax
    mov es, ax

    mov eax, [esp + 44] 
    push eax
    call isr_handler_c
    add esp, 4

    pop es
    pop ds
    popa
    iretd

; -------------------------------------------------------
; IDT loader
; -------------------------------------------------------
idt_load:
    mov eax, [esp + 4]
    lidt [eax]
    ret