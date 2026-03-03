; -------------------------------------------------------------------------
;                                   AurorOS
; (c) 2022-2025 Interpuce
; 
; You should receive AurorOS license with this source code. If not - check:
;  https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md
; -------------------------------------------------------------------------

; todo: make an x86 compatibile reboot

internal_qemu_reboot:
    ret
    
internal_qemu_shutdown:
    ret 

global internal_qemu_reboot
global internal_qemu_shutdown
