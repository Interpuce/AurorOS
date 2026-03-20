; todo: make an x86/x86_64 compatibile reboot

internal_qemu_reboot:
    ret
    
internal_qemu_shutdown:
    ret 

global internal_qemu_reboot
global internal_qemu_shutdown
