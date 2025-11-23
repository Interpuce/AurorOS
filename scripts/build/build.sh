#!/bin/bash

# Begin with compile-time dependencies check

set -e

dependencies_required=("gcc" "grub-mkrescue" "nasm" "ld" "xorriso" "mtools") # REMINDER: Remember to update this when a new tool is introduced to the toolchain.
dependencies_missing=()

for utility in "${dependencies_required[@]}"; do
    if ! command -v "$utility" &>/dev/null; then
        if [ ${#dependencies_missing[@]} -gt 0 ]; then
            echo "Some dependencies are missing!"
        fi
        sleep 0.2        
        dependencies_missing+=("$utility")  # Add any missing utilities to the list
    fi
done

# Call for ABDM (AurorOS Build-time Dependencies Manager, a.k.a. dep_install.sh) if anything's missing.
if [ ${#dependencies_missing[@]} -gt 0 ]; then
	    ./compile/dep_install.sh
fi

ROOT_DIR="."
SRC_DIR="$ROOT_DIR/src"
LINKER_SCRIPT="$SRC_DIR/kernel/linker.ld"
KERNEL_OUTPUT="$ROOT_DIR/kernel.bin"
ISO_OUTPUT="$ROOT_DIR/AurorOS.iso"
ISO_DIR="$ROOT_DIR/iso"
BOOT_DIR="$ISO_DIR/boot"
GRUB_DIR="$BOOT_DIR/grub"
OBJECT_DIR="$ROOT_DIR/bin"

mkdir -p "$OBJECT_DIR"

OBJECT_FILES=()
echo -e "[\033[1m\033[33m*\033[0m] C sources -> objects"
for SOURCE_FILE in $(find "$SRC_DIR" -type f -name '*.c' ! -name '*.excluded.c'); do
    OBJECT_FILE="$OBJECT_DIR/$(basename "${SOURCE_FILE%.c}.o")"
    OBJECT_FILES+=("$OBJECT_FILE")
    echo -e "[\033[1m\033[36m*\033[0m] $SOURCE_FILE -> $OBJECT_FILE"
    gcc -Wall -Wextra -m32 -ffreestanding -nostartfiles -Iinclude -nostdlib -fno-stack-protector -c "$SOURCE_FILE" -o "$OBJECT_FILE"
done

echo -e "[\033[1m\033[33m*\033[0m] assembly sources -> objects"
for ASM_FILE in $SRC_DIR/**/*.asm; do
    [ -e "$ASM_FILE" ] || continue
    BASENAME=$(basename "$ASM_FILE" .asm)
    OBJ_FILE="$OBJECT_DIR/${BASENAME}.o"
    echo -e "[\033[1m\033[36m*\033[0m] $ASM_FILE -> $OBJ_FILE"
    nasm -f elf32 "$ASM_FILE" -o "$OBJ_FILE"
    OBJECT_FILES+=("$OBJ_FILE")
done

echo -e "[\033[1m\033[33m*\033[0m] objects -> kernel binary"
echo -e "[\033[1m\033[36m*\033[0m] .o -> .bin"
ld -m elf_i386 -T "$LINKER_SCRIPT" -o "$KERNEL_OUTPUT" "${OBJECT_FILES[@]}"

echo -e "[\033[1m\033[33m*\033[0m] kernel binary -> ISO"
echo -e "[\033[1m\033[36m*\033[0m] .bin -> AurorOS.iso"
mkdir -p "$GRUB_DIR"
cp "$KERNEL_OUTPUT" "$BOOT_DIR"
cat > "$GRUB_DIR/grub.cfg" << EOF
menuentry "AurorOS" {
    multiboot /boot/kernel.bin
}

set default=0
set timeout=5
EOF

grub-mkrescue -o "$ISO_OUTPUT" "$ISO_DIR"

echo -e "\033[32mSuccess!\033[0m"
