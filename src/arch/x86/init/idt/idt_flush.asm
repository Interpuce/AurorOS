global idt_flush
idt_flush:
    lidt [eax]
    ret

global irq12_entry
global irq_entry
extern irq12_handler

irq12_entry:
    pushad 
    push ds
    push es
    push fs
    push gs

    mov ax, 0x10  
    mov ds, ax
    mov es, ax

    call irq12_handler

    mov al, 0x20
    out 0xA0, al
    out 0x20, al   

    pop gs
    pop fs
    pop es
    pop ds
    popad
    iretd    

irq_entry:
    mov al, 0x20
    out 0xA0, al
    out 0x20, al
    iretd
