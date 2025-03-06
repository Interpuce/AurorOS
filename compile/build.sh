#!/bin/bash

# Please make sure you have acknowledged AurorOS license.
# It is available at https://github.com/Interpuce/AurorOS/blob/main/LICENSE.md

# Begin with compile-time dependencies check

set -e

dependencies_required=("gcc" "grub-mkrescue" "nasm" "ld" "xorriso" "git")
dependencies_missing=()

for utility in "${dependencies_required[@]}"; do
    if ! command -v "$utility" &>/dev/null; then
        echo "Some dependencies are missing!"
        echo "If you are building AurorOS in Termux please stop! It won't work."
        sleep 0.2        
        dependencies_missing+=("$utility")
    fi
done

if [ ${#dependencies_missing[@]} -gt 0 ]; then
    ./compile/dep_install.sh
fi

ROOT_DIR=$(pwd)
ASM_FILE="$ROOT_DIR/kernel/boot/boot.asm"
ASM_OBJECT="$ROOT_DIR/bin/boot.o"
LINKER_KERNEL_SCRIPT="$ROOT_DIR/compile/linker_kernel.ld"
LINKER_USER_SCRIPT="$ROOT_DIR/compile/linker_user.ld"
KERNEL_OUTPUT="$ROOT_DIR/kernel.bin"
USER_OUTPUT="$ROOT_DIR/user.bin"
ISO_OUTPUT="$ROOT_DIR/AurorOS.iso"
ISO_DIR="iso"
BOOT_DIR="$ISO_DIR/boot"
GRUB_DIR="$BOOT_DIR/grub"
OBJECT_DIR="$ROOT_DIR/bin"

mkdir -p "$OBJECT_DIR"

# Kernel mode compilation
echo ".c -> .o [kernel-mode]"
KERNEL_OBJECT_FILES=()
for SOURCE_FILE in $(find "$ROOT_DIR/kernel/" -type f -name '*.c' ! -name '*.excluded.c'); do
    OBJECT_FILE="$OBJECT_DIR/$(basename "${SOURCE_FILE%.c}.o")"
    KERNEL_OBJECT_FILES+=("$OBJECT_FILE")
    gcc -Wall -Wextra -m32 -ffreestanding -nostartfiles -Ikernel/include -nostdlib -fno-stack-protector -c "$SOURCE_FILE" -o "$OBJECT_FILE"
done

# User mode compilation
echo ".c -> .o [user-mode]"
if [ ! -d "$ROOT_DIR/apps/stdlib" ]; then
    echo "Directory apps/stdlib does not exist. Cloning repository (shadow clone, not for contributing)..."
    git clone https://github.com/Interpuce/stdlib "$ROOT_DIR/apps/stdlib" --depth 1
fi

USER_OBJECT_FILES=()
for SOURCE_FILE in $(find "$ROOT_DIR/apps/" -type f -name '*.c' ! -name '*.excluded.c'); do
    OBJECT_FILE="$OBJECT_DIR/$(basename "${SOURCE_FILE%.c}.o")"
    USER_OBJECT_FILES+=("$OBJECT_FILE")
    gcc -Wall -Wextra -m32 -ffreestanding -nostartfiles -Iapps/stdlib/include -nostdlib -c "$SOURCE_FILE" -o "$OBJECT_FILE"
done

echo "boot.asm -> boot.o"
nasm -f elf32 "$ASM_FILE" -o "$ASM_OBJECT"

# Linking kernel
echo ".o -> kernel.bin"
ld -m elf_i386 -T "$LINKER_KERNEL_SCRIPT" -o "$KERNEL_OUTPUT" "${KERNEL_OBJECT_FILES[@]}" "$ASM_OBJECT"

# Linking user-space programs
echo ".o -> user.bin"
ld -m elf_i386 -T "$LINKER_USER_SCRIPT" -o "$USER_OUTPUT" "${USER_OBJECT_FILES[@]}"

# Creating ISO
echo ".bin -> AurorOS.iso"
mkdir -p "$GRUB_DIR"
cp "$KERNEL_OUTPUT" "$BOOT_DIR"
cp "$USER_OUTPUT" "$BOOT_DIR"
cat > "$GRUB_DIR/grub.cfg" << EOF
menuentry "AurorOS" {
    multiboot /boot/kernel.bin
}
EOF

grub-mkrescue -o "$ISO_OUTPUT" "$ISO_DIR"

echo -e "\033[32mSuccess!\033[0m"