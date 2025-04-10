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

; boot.asm configures everything (including Multiboot Header required by GRUB
; and other bootloaders to see AurorOS) and loads the kernel main function
; from /main/main.c

bits 16

section .multiboot
    align 4
    dd 0x1BADB002
    dd 0x00
    dd -(0x1BADB002 + 0x00)

section .text

extern main
extern idt_install
extern init_gdt
global start

start:
    cli 
    call init_gdt
    call idt_install
    mov eax, cr0 
    or eax, 1 
    mov cr0, eax
    jmp 08h:protected_mode_start

bits 32

protected_mode_start:
    call main
    hlt