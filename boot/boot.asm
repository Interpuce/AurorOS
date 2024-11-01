; -------------------------------------------------------------------------
;                                   AurorOS
; (c) 2022-2024 Interpuce
;
; You should receive AurorOS license with this source code. If not - check:
;  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
; -------------------------------------------------------------------------

bits 32
section .text
        align 4
        dd 0x1BADB002
        dd 0x00
        dd - (0x1BADB002+0x00)
        
global start
extern main
start:
        cli
        call main
        hlt
