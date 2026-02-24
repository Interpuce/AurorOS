#!/usr/bin/env -S bash --posix

# this script launches AurorOS in gdb debugging mode
#
# debugging mode involves using gdb to debug the kernel
# 
# this script is a fancy and handy alias to set
# up remote debugging on port 1234 and start AurorOS
# from ISO

qemu-system-i386 -s -S -cdrom AurorOS.iso >/dev/null 2>&1 & disown

gdb -ex "target remote :1234" ./iso/boot/kernel.bin
