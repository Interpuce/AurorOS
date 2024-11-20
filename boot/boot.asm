; -------------------------------------------------------------------------
;                                   AurorOS
; (c) 2022-2024 Interpuce
;
; You should receive the AurorOS license with this source code. If not, check:
;  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
; -------------------------------------------------------------------------

; AurorOS is designed to work on a 32-bit x86 architecture.
; It currently does not support other architectures, but support for
; ARM and PowerPC is planned for the future.

bits 32                   ; This code is written for 32-bit x86 architecture.

section .text             ; Define the start of the text section (code segment).
        align 4           ; Ensure alignment to a 4-byte boundary for performance.

        ; Multiboot Header:
        ; Required by the Multiboot specification to identify the kernel as
        ; multiboot-compliant. This allows bootloaders like GRUB to recognize
        ; and correctly load the kernel.

        dd 0x1BADB002     ; Magic number indicating Multiboot compliance.
        dd 0x00           ; Flags (0 = no additional features required).
        dd - (0x1BADB002 + 0x00) ; Checksum: Ensures the sum of all three fields is 0.

section .data             ; Define the start of the data section (initialized data).
        align 4           ; Ensure alignment to a 4-byte boundary.

        ; Interrupt Descriptor Table (IDT):
        ; The IDT stores interrupt vectors and their corresponding handlers.
        idt:
        times 256 dq 0    ; Reserve space for 256 IDT entries (8 bytes per entry).

        ; IDT Descriptor:
        ; Used to load the IDT with the 'lidt' instruction.
        idt_descriptor:
        dw idt_end - idt - 1  ; Limit (size of the IDT - 1).
        dd idt                ; Base address of the IDT.

        idt_end:

; Define the global entry point of the OS.
global start              ; 'start' is the entry point defined in the linker script.

; Declare an external symbol for the kernel load function.
; This function is implemented in C, in the file 'boot/load.c'.
extern kernel_load

; Macro for setting an IDT entry.
; Parameters:
; %1 - Interrupt number
; %2 - Handler address
; %3 - Segment selector (e.g., code segment selector).
%macro set_idt_entry 3
    mov eax, %2                        ; Load the handler address into EAX.
    mov [idt + %1 * 8], ax             ; Set the low 16 bits of the handler address.
    mov word [idt + %1 * 8 + 2], %3    ; Set the segment selector.
    mov byte [idt + %1 * 8 + 4], 0     ; Reserved (must be zero).
    mov byte [idt + %1 * 8 + 5], 0x8E  ; Attributes: Present, Ring 0, 32-bit interrupt gate.
    shr eax, 16                        ; Shift right to get the high 16 bits of the address.
    mov word [idt + %1 * 8 + 6], ax    ; Set the high 16 bits of the handler address.
%endmacro

init_idt:
        ; Nothing for now

; Entry point of the bootloader stage.
start:
        cli               ; Disable interrupts to ensure a controlled initialization.
        call kernel_load  ; Call the kernel load function to initialize the kernel.
        call init_idt     ; Initialize the IDT and enable interrupt handling.
        hlt               ; Halt the CPU to stop execution (until an interrupt occurs).