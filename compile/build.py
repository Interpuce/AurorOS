#!/usr/bin/env python3

import os
import subprocess
import sys

def compile_c_files(root_dir):
    object_files = []
    for dirpath, _, filenames in os.walk(root_dir):
        for file in filenames:
            if file.endswith('.c'):
                if file.endswith('.excluded.c'):
                    continue
                
                source_path = os.path.join(dirpath, file)
                object_path = source_path[:-2] + '.o'
                object_files.append(object_path)
                
                try:
                    subprocess.run(
                        ["gcc", "-Wall", "-Wextra", "-m32", "-ffreestanding", "-nostartfiles", "-Iinclude", "-nostdlib", "-fno-stack-protector", "-c", source_path, "-o", object_path],
                        check=True
                    )
                except subprocess.CalledProcessError:
                    sys.exit(1)
    return object_files

def compile_asm_file(asm_file, output_file):
    try:
        subprocess.run(
            ["nasm", "-f", "elf32", asm_file, "-o", output_file],
            check=True
        )
    except subprocess.CalledProcessError:
        sys.exit(1)

def link_object_files(object_files, asm_object, linker_script, output_file):
    try:
        subprocess.run(
            ["ld", "-m", "elf_i386", "-T", linker_script, "-o", output_file, *object_files, asm_object],
            check=True
        )
    except subprocess.CalledProcessError:
        sys.exit(1)

def create_iso(kernel, iso_name):
    iso_dir = "iso"
    boot_dir = os.path.join(iso_dir, "boot")
    grub_dir = os.path.join(boot_dir, "grub")
    
    os.makedirs(grub_dir, exist_ok=True)
    subprocess.run(["cp", kernel, boot_dir], check=True)
    
    grub_cfg_path = os.path.join(grub_dir, "grub.cfg")
    with open(grub_cfg_path, "w") as grub_cfg:
        grub_cfg.write('menuentry "aurorOS 0.0.1" {\n    multiboot /boot/kernel.bin\n}')
    
    try:
        subprocess.run(["grub-mkrescue", "-o", iso_name, iso_dir], check=True)
    except subprocess.CalledProcessError:
        sys.exit(1)

def main():
    root_dir = os.getcwd()
    asm_file = os.path.join(root_dir, "boot", "boot.asm")
    asm_object = os.path.join(root_dir, "boot", "boot.o")
    linker_script = os.path.join(root_dir, "compile", "linker.ld")
    kernel_output = os.path.join(root_dir, "kernel.bin")
    iso_output = os.path.join(root_dir, "aurorOS.iso")
    
    print("Compiling C files...")
    object_files = compile_c_files(root_dir)
    
    print("Compiling assembly file...")
    compile_asm_file(asm_file, asm_object)
    
    print("Linking object files...")
    link_object_files(object_files, asm_object, linker_script, kernel_output)
    
    print("Creating ISO...")
    create_iso(kernel_output, iso_output)
    print(f"ISO created: {iso_output}")

if __name__ == "__main__":
    main()