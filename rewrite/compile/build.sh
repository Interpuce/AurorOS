#!/bin/bash

# Please make sure you have acknowledged AurorOS license.
# It is available at https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md

# STEP 1: Begin with compile-time dependencies check

set -e

dependencies_required=("gcc" "grub-mkrescue" "nasm" "ld" "xorriso" "git")

for utility in "${dependencies_required[@]}"; do
    if ! command -v "$utility" &>/dev/null; then
        echo "Some dependencies are missing!"
        echo "If you are building AurorOS in Termux please stop! It won't work, even with proot-distro."
        sleep 0.2
        echo "It is recommended to run dep_install.sh in case of missing dependencies."
        echo "You can run it by typing 'make dep_install' in your shell."
        if [[ "$*" != *"-force-skip-depman"* ]]; then
            exit 1
        fi
    fi
done

ROOT_DIR=$(pwd)
KERNEL_DIR="$ROOT_DIR/kernel"
KERNEL_ASSEMBLY_DIR="$KERNEL_DIR/asm"
KERNEL_OUTPUT_BINARY="$ROOT_DIR/out/kernel.bin"
BOOT_DIR="$ISO_DIR/boot"
GRUB_DIR="$BOOT_DIR/grub"
OBJECT_DIR="$ROOT_DIR/bin"
ISO_OUTPUT="$ROOT_DIR/AurorOS.iso"
ISO_DIR="iso"

# STEP 2: Build the kernel

echo ".c -> .o [kernel-mode]"
KERNEL_OBJECT_FILES=()
for SOURCE_FILE in $(find "$KERNEL_DIR" -type f -name '*.c' ! -name '*.excluded.c'); do
    OBJECT_FILE="$OBJECT_DIR/$(basename "${SOURCE_FILE%.c}.o")"
    KERNEL_OBJECT_FILES+=("$OBJECT_FILE")
    gcc -Wall -Wextra -m32 -ffreestanding -nostartfiles -Ikernel/include -nostdlib -fno-stack-protector -c "$SOURCE_FILE" -o "$OBJECT_FILE"
done

echo ".asm -> .o [kernel-mode]"
for SOURCE_FILE in $(find "$KERNEL_DIR" -type f -name '*.asm' ! -name '*.excluded.asm'); do
    OBJECT_FILE="$OBJECT_DIR/$(basename "${SOURCE_FILE%.asm}.o")"
    KERNEL_OBJECT_FILES+=("$OBJECT_FILE")
    nasm -f elf32 "$SOURCE_FILE" -o "$OBJECT_FILE"
done

# STEP 3: Linking kernel
echo ".o -> kernel.bin"
ld -m elf_i386 -T "$ROOT_DIR/compile/kernel-linker.ld" -o "$KERNEL_OUTPUT_BINARY" "${KERNEL_OBJECT_FILES[@]}"

# LAST STEP: Creating ISO
echo ".bin -> AurorOS.iso"
mkdir -p "$GRUB_DIR"
cp "$KERNEL_OUTPUT_BINARY" "$BOOT_DIR"

cat > "$GRUB_DIR/grub.cfg" << EOF
menuentry "AurorOS" {
    multiboot /boot/kernel.bin
}
EOF

grub-mkrescue -o "$ISO_OUTPUT" "$ISO_DIR"

echo -e "\033[32mSuccess!\033[0m"