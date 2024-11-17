; -------------------------------------------------------------------------
;                                   AurorOS
; (c) 2022-2024 Interpuce
;
; You should receive AurorOS license with this source code. If not - check:
;  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
; -------------------------------------------------------------------------

; The AurorOS works in 32-bit architecture and currently does not support
; any other architecture. We want to support ARM and PowerPC in the future.

bits 32                   ; This code is written for a 32-bit architecture

section .text             ; Define the start of the text section
        align 4           ; Align the following data to a 4-byte boundary

        ; Multiboot Header:
        ; The Multiboot specification requires a specific header to identify
        ; the kernel as a multiboot-compliant image. This enables bootloaders
        ; like GRUB to properly load the kernel.

        dd 0x1BADB002     ; Magic number indicating Multiboot compliance
        dd 0x00           ; Flags (0 means no additional features required)
        dd - (0x1BADB002+0x00) ; Checksum: Ensures that the sum of the three
                               ; header fields is 0

; Define the global entry point of the OS
global start              ; Marks the 'start' label as the entry point of this file.
                          ; This is also defined in the linker script (compile/linker.ld).

; Declare an external symbol for the kernel load function
; This function is implemented in C, in the file boot/load.c
extern kernel_load        

; Entry point of the bootloader stage
start:
        cli               ; Clear interrupts to ensure a controlled environment
        call kernel_load  ; Call the kernel_load function to initialize the kernel
        hlt               ; Halt the CPU