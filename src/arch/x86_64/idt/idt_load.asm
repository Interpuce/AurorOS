bits 64

global isr_stub_table
global irq_stub_table
global idt_load
extern isr_handler

%macro ISR_NOERR 1
isr%1:
    push 0
    push %1
    jmp isr_common
%endmacro

%macro ISR_ERR 1
isr%1:
    push %1
    jmp isr_common
%endmacro

%macro IRQ 1
irq%1:
    push 0
    push (32 + %1)
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

bits 64

extern isr_handler
global isr_common

isr_common:
    push rax
    push rbx
    push rcx
    push rdx
    push rbp
    push rdi
    push rsi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    mov rdi, rsp        
    call isr_handler

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rsi
    pop rdi
    pop rbp
    pop rdx
    pop rcx
    pop rbx
    pop rax

    add rsp, 16  
    iretq

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
    mov rax, rdi
    lidt [rax]
    ret
