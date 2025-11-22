; -------------------------------------------------------------------------
;                                   AurorOS
; (c) 2022-2025 Interpuce
; 
; You should receive AurorOS license with this source code. If not - check:
;  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
; -------------------------------------------------------------------------

internal_qemu_reboot:
    mov ax, 0xfee1
    mov bx, 0xdead
    int 0x19

internal_qemu_shutdown:
    mov ax, 0x2000
    mov dx, 0x604
    out dx, ax

global internal_qemu_reboot
global internal_qemu_shutdown