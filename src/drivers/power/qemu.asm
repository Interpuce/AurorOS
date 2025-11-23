; -------------------------------------------------------------------------
;                                   AurorOS
; (c) 2022-2025 Interpuce
; 
; You should receive AurorOS license with this source code. If not - check:
;  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
; -------------------------------------------------------------------------

; These power functions work only in QEMU and only in real mode.
; If IDT was ever introducted, it'll have to be disabled to continue
; properly manage the power of the virtual machine.

internal_qemu_reboot:
    mov ax, 0xfee1
    mov bx, 0xdead
    int 0x19 ; uses BIOS interrupts, so can be used only
             ; in real/unreal mode.

; This basically behaves like outw in AT&T assembler, but the rest of
; the syntax of that assembler is terrible, so I decided to not use it.
internal_qemu_shutdown:
    mov ax, 0x2000
    mov dx, 0x604
    out dx, ax

global internal_qemu_reboot
global internal_qemu_shutdown