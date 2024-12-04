# AurorOS Makefile

# This file redirects the compilation to a Python script
#  located in /compile/build.py, which goal is to build
#  correctly AurorOS.

.PHONY = all

all:
	python3 compile/build.py