# AurorOS Makefile

.PHONY: all help bugreport shbuild depinstall

all: shbuild

help:
	@echo "To compile the AurorOS, please run make."
	@echo "To force installing dependencies, run make depinstall."

bugreport:
	@echo "To report a bug, contact developers at https://github.com/Interpuce/AurorOS/"
	@echo "Do it by opening an issue."

shbuild:
	chmod +x compile/build.sh
	chmod +x compile/dep_install.sh
	./compile/build.sh

depinstall:
	chmod +x compile/dep_install.sh
	./compile/dep_install.sh
