; ------------------------------------------------------------
;                   AurorOS bootloader
;
; The main entry to the OS where it loads kernel.
; ------------------------------------------------------------

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
