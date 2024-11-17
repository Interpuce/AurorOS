# AurorOS build file
# (c) 2022-2024 Interpuce

# DO NOT LEARN HOW TO COMPILE OS FROM THIS FILE
# Modifying this file may cause AurorOS to not build correctly.

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
                        ["gcc", "-m32", "-ffreestanding", "-nostartfiles", "-Iinclude", "-nostdlib", "-c", source_path, "-o", object_path],
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

def main():
    root_dir = os.getcwd()
    asm_file = os.path.join(root_dir, "boot", "boot.asm")
    asm_object = os.path.join(root_dir, "boot", "boot.o")
    linker_script = os.path.join(root_dir, "compile", "linker.ld")
    output_file = os.path.join(root_dir, "kernel.bin")
    
    object_files = compile_c_files(root_dir)
    compile_asm_file(asm_file, asm_object)
    link_object_files(object_files, asm_object, linker_script, output_file)

if __name__ == "__main__":
    main()
