#!/bin/bash

cd "$(dirname "$0")" || exit 1

gcc="gcc"
nasm="nasm"
flags="-Wall -Wextra -m64 -march=x86-64 -ffreestanding -Iinclude"
ldflags="-T ./linker.ld -nostdlib -nostartfiles"

mkdir -p bin iso

find . -name "*.c" | while read -r file; do
    base_name="${file%.c}"

    $gcc $flags -c "$file" -o "bin/$base_name.o"

    if [ $? -eq 0 ]; then
        echo "Compiled $file to bin/$base_name.o"
    else
        echo "Failed to compile $file"
        exit 1
    fi
done

$nasm -f bin boot/start.asm -o bin/start.bin

if [ $? -eq 0 ]; then
    echo "Compiled boot/start.asm to bin/start.bin"
else
    echo "Failed to compile boot/start.asm"
    exit 1
fi

$gcc $ldflags -o bin/kernel.bin bin/*.o

if [ $? -eq 0 ]; then
    echo "Linked to kernel.bin successfully."
else
    echo "Failed to link to kernel.bin."
    exit 1
fi

cat bin/start.bin bin/kernel.bin > iso/kernel.bin

genisoimage -o auroros.iso -b kernel.bin -no-emulation -boot-load-size 4 -boot-info-table iso

if [ $? -eq 0 ]; then
    echo "ISO image created successfully: auroros.iso"
else
    echo "Failed to create ISO image."
    exit 1
fi