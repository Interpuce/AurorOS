#!/bin/bash

# Please make sure you have acknowledged AurorOS license.
# It is available at https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md

# Begin with compile-time dependencies check

set -e

dependencies_required=("gcc" "grub-mkrescue" "nasm" "ld" "xorriso" "git") # REMINDER: Remember to update this when a new tool is introduced to the toolchain.
dependencies_missing=()

for utility in "${dependencies_required[@]}"; do
    if ! command -v "$utility" &>/dev/null; then
	echo "Some dependencies are missing!"
	sleep 0.2        
	dependencies_missing+=("$utility")  # Add any missing utilities to the list
    fi
done

# Call for ABDM (AurorOS Build-time Dependencies Manager, a.k.a. dep_install.sh) if anything's missing.
if [ ${#dependencies_missing[@]} -gt 0 ]; then
	    ./compile/dep_install.sh
fi

ROOT_DIR=$(pwd)
ASM_FILE="$ROOT_DIR/kernel/boot/boot.asm"
ASM_OBJECT="$ROOT_DIR/kernel/bin/boot.o"
LINKER_SCRIPT="$ROOT_DIR/compile/linker.ld"
KERNEL_OUTPUT="$ROOT_DIR/kernel.bin"
ISO_OUTPUT="$ROOT_DIR/AurorOS.iso"
ISO_DIR="iso"
BOOT_DIR="$ISO_DIR/boot"
GRUB_DIR="$BOOT_DIR/grub"
OBJECT_DIR="$ROOT_DIR/bin"

mkdir -p "$OBJECT_DIR"

echo ".c -> .o [kernel-level]"
OBJECT_FILES=()
for SOURCE_FILE in $(find "$ROOT_DIR/kernel/" -type f -name '*.c' ! -name '*.excluded.c'); do
    OBJECT_FILE="$OBJECT_DIR/$(basename "${SOURCE_FILE%.c}.o")"
    OBJECT_FILES+=("$OBJECT_FILE")
    gcc -Wall -Wextra -m32 -ffreestanding -nostartfiles -Ikernel/include -nostdlib -fno-stack-protector -c "$SOURCE_FILE" -o "$OBJECT_FILE"
done

if [ ! -d "$ROOT_DIR/apps/include" ]; then
    echo "Directory apps/include does not exist. Cloning repository..."
    git clone https://github.com/Interpuce/stdlib "$ROOT_DIR/apps/stdlib"
fi

echo ".c -> .o [user-level]"
for SOURCE_FILE in $(find "$ROOT_DIR/apps/" -type f -name '*.c' ! -name '*.excluded.c'); do
    OBJECT_FILE="$OBJECT_DIR/$(basename "${SOURCE_FILE%.c}.o")"
    OBJECT_FILES+=("$OBJECT_FILE")
    gcc -Wall -Wextra -m32 -ffreestanding -nostartfiles -Iapps/stdlib/include -nostdlib  -c "$SOURCE_FILE" -o "$OBJECT_FILE"
done

echo "boot.asm -> boot.o"
nasm -f elf32 "$ASM_FILE" -o "$ASM_OBJECT"

echo ".o -> .bin"
ld -m elf_i386 -T "$LINKER_SCRIPT" -o "$KERNEL_OUTPUT" "${OBJECT_FILES[@]}" "$ASM_OBJECT"

echo ".bin -> AurorOS.iso"
mkdir -p "$GRUB_DIR"
cp "$KERNEL_OUTPUT" "$BOOT_DIR"
cat > "$GRUB_DIR/grub.cfg" << EOF
menuentry "AurorOS" {
    multiboot /boot/kernel.bin
}
EOF

grub-mkrescue -o "$ISO_OUTPUT" "$ISO_DIR"

echo -e "\033[32mSuccess!\033[0m"
