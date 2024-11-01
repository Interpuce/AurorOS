# simple build tool

nasm -f elf32 boot/boot.asm -o bootloader.o
gcc -m32 -c kernel/kernel.c -o kernel.o -ffreestanding
ld -m elf_i386 -T compile/linker.ld -o kernel.bin bootloader.o kernel.o 

qemu-system-x86_64 -kernel kernel.bin
