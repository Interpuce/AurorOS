# AurorOS Makefile

.PHONY: all help shbuild depinstall clean run_qemu

all: shbuild

help:
	@echo "To compile the AurorOS, please run `make`."
	@echo "To force installing dependencies, run `make depinstall`."

ifeq ($(OS), Windows_NT)
shbuild:
	cmd /c scripts\build\win\build.bat

depinstall:
	cmd /c scripts\build\win\build.bat

clean:
	rd /s /q bin
	rd /s /q iso
	del kernel.bin
	del AurorOS.iso
else
shbuild:
	chmod +x scripts/build/build.sh
	chmod +x scripts/build/dep_install.sh
	./scripts/build/build.sh

depinstall:
	chmod +x scripts/build/dep_install.sh
	./scripts/build/dep_install.sh

clean: 
	rm -rf bin
	rm -rf iso
	rm -rf kernel.bin
	rm -rf AurorOS.iso

run_qemu:
	qemu-system-x86_64 -cdrom AurorOS.iso
endif