#!/bin/bash

set -e

ROOT_DIR=$(pwd)
ASM_FILE="$ROOT_DIR/boot/boot.asm"
ASM_OBJECT="$ROOT_DIR/bin/boot.o"
LINKER_SCRIPT="$ROOT_DIR/compile/linker.ld"
KERNEL_OUTPUT="$ROOT_DIR/kernel.bin"
ISO_OUTPUT="$ROOT_DIR/AurorOS.iso"
ISO_DIR="iso"
BOOT_DIR="$ISO_DIR/boot"
GRUB_DIR="$BOOT_DIR/grub"
OBJECT_DIR="$ROOT_DIR/bin"

mkdir -p "$OBJECT_DIR"

echo ".c -> .o"
OBJECT_FILES=()
for SOURCE_FILE in $(find "$ROOT_DIR" -type f -name '*.c' ! -name '*.excluded.c'); do
    OBJECT_FILE="$OBJECT_DIR/$(basename "${SOURCE_FILE%.c}.o")"
    OBJECT_FILES+=("$OBJECT_FILE")
    gcc -Wall -Wextra -m32 -ffreestanding -nostartfiles -Iinclude -nostdlib -fno-stack-protector -c "$SOURCE_FILE" -o "$OBJECT_FILE"
done

echo "boot.asm -> boot.o"
nasm -f elf32 "$ASM_FILE" -o "$ASM_OBJECT"

echo ".o -> .bin"
ld -m elf_i386 -T "$LINKER_SCRIPT" -o "$KERNEL_OUTPUT" "${OBJECT_FILES[@]}" "$ASM_OBJECT"

echo ".bin -> AurorOS.iso"
mkdir -p "$GRUB_DIR"
cp "$KERNEL_OUTPUT" "$BOOT_DIR"
cat > "$GRUB_DIR/grub.cfg" << EOF
menuentry "AurorOS 0.0.1" {
    multiboot /boot/kernel.bin
}
EOF

grub-mkrescue -o "$ISO_OUTPUT" "$ISO_DIR"

echo "Success!"
