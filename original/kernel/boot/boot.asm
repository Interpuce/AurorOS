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
; from /kernel/kernel.c

bits 32

; Multiboot Header section. Required by some bootloaders (including GRUB, that
; AurorOS uses in the current state) to see binary as operating system.
section .multiboot
    align 4
    dd 0x1BADB002
    dd 0x00
    dd -(0x1BADB002 + 0x00)

section .text

; The main function where AurorOS starts. It is also declared in
; linker script (/compile/linker.ld), defined in /kernel/kernel.c
; file.
global start
extern main

start:
    cli ; Clear interrupts (i thought i forgot) 
    call main ; Call the main function (from /kernel/loader/kernel.c)
    hlt ; Halt the proccessor

; eclair was here (twice)
; gorciu was here (three times)

global idt_load
extern idtp ; kernel/idt/idt.c
idt_load:
    lidt [idtp]
    sti
    ret

; new file end