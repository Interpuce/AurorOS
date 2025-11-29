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

; Multiboot Header section
section .multiboot
    align 4
    dd 0x1BADB002
    dd 0x00
    dd -(0x1BADB002 + 0x00)

section .text

global arch_x86_start
extern arch_x86_real_mode_entry
extern _kernel_stack_end

arch_x86_start:
    cli
    mov esp, _kernel_stack_end
    mov ebp, esp
    call arch_x86_real_mode_entry
    hlt

; eclair was here (twice)
; gorciu was here