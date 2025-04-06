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

extern idtp ; kernel/drivers/idt/idt.c
global idt_load
idt_load:
    lidt [idtp]
    sti
    ret