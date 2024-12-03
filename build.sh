#!/bin/bash
set -e

KERNEL=kernel.bin

gcc -m32 -c kernel/kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c kernel/ports.c -o ports.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c kernel/commands/commands.c -o commands.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

gcc -m32 -c include/string.c -o string.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c include/screen.c -o screen.o -std=gnu99 -ffreestanding -fno-stack-protector -O2 -Wall -Wextra
gcc -m32 -c include/input.c -o input.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
gcc -m32 -c include/msg.c -o msg.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

nasm -f elf32 boot/boot.asm -o boot.o
ld -m elf_i386 -T linker.ld -o $KERNEL boot.o kernel.o string.o screen.o ports.o input.o msg.o commands.o

mkdir -p iso/boot/grub
cp $KERNEL iso/boot/
cat > iso/boot/grub/grub.cfg << EOF
menuentry "aurorOS 0.0.1" {
    multiboot /boot/kernel.bin
}
EOF
grub-mkrescue -o aurorOS.iso iso

rm -r isodir
