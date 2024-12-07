# AurorOS Makefile

# This file redirects the compilation to a Python script
#  located in /compile/build.py, which goal is to build
#  correctly AurorOS.

# To correctly compile AurorOS, you will need `gcc`, `nasm`
#  and in current state - `grub-mkrescue`. More about compilation
#  requirements you can learn in ./CONTRIBUTING.md

.PHONY = all shbuild

all:
	python3 compile/build.py

shbuild:
	chmod -x compile/build.sh
	./compile/build.sh