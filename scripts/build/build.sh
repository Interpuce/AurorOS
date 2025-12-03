#!/bin/bash

# Begin with compile-time dependencies check

set -e
shopt -s globstar

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
	    ./scripts/build/dep_install.sh
fi

ROOT_DIR="."
SRC_DIR="$ROOT_DIR/src"
LINKER_SCRIPT="$SRC_DIR/arch/x86/init/linker.ld"
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

echo -e "[\033[1m\033[33m*\033[0m] C++ sources -> objects"
for SOURCE_FILE in $(find "$SRC_DIR" -type f -name '*.cpp' ! -name '*.excluded.cpp'); do
    OBJECT_FILE="$OBJECT_DIR/$(basename "${SOURCE_FILE%.cpp}.o")"
    OBJECT_FILES+=("$OBJECT_FILE")
    echo -e "[\033[1m\033[36m*\033[0m] $SOURCE_FILE -> $OBJECT_FILE"
    g++ -Wall -Wextra -m32 -ffreestanding -fno-rtti -fno-threadsafe-statics -nostartfiles -Iinclude -nostdlib -fno-stack-protector -fno-exceptions -fno-rtti -c "$SOURCE_FILE" -o "$OBJECT_FILE"
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
set gfxmode=1024x768x32
set gfxpayload=keep
load_video
terminal_output gfxterm

menuentry "AurorOS" {
    multiboot /boot/kernel.bin
}

set default=0
set timeout=5
EOF

grub-mkrescue -o "$ISO_OUTPUT" "$ISO_DIR"

echo -e "\033[32mSuccess!\033[0m"
