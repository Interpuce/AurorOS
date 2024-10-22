# ------------------------------------------------------------
#                   AurorOS bootloader
#
# The main entry to the OS where it loads kernel.
# ------------------------------------------------------------

section .text
    extern load_kernel
    global _start

_start:
    call load_kernel

    mov rax, 60
    xor rdi, rdi
    syscall