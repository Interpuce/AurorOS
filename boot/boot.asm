; -------------------------------------------------------------------------
;                                   AurorOS
; (c) 2022-2024 Interpuce
;
; You should receive AurorOS license with this source code. If not - check:
;  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
; -------------------------------------------------------------------------

; The AurorOS works in 32-bit architecture and currently does not support
; any other architecture

bits 32
section .text
        align 4
        dd 0x1BADB002
        dd 0x00
        dd - (0x1BADB002+0x00)
        
global start
extern kernel_load ; Declaration of kernel_load in boot/load.c
start:
        cli
        call kernel_load
        hlt ; Halt the processor
