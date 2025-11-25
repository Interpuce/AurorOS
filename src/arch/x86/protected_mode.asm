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

; This file does not set up GDT & IDT tables.
; For that logic please see src/main/main.c

bits 32

extern protected_mode_entry

global protected_mode_init

protected_mode_init:
    ; enables PE bit in cr0
    ;   CR0[0] = Protection Enable
    mov eax, cr0
    or eax, 1 
    mov cr0, eax
    ; something
    mov ax, 0x18
    ltr ax
    ; far jump to the protected mode entry
    jmp 0x08:protected_mode_entry
