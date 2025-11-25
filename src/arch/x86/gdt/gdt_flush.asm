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

global gdt_flush

gdt_flush:
    mov eax, [esp + 4] 
    lgdt [eax]

    mov ax, 0x10 
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret