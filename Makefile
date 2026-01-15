all:
	@chmod +x scripts/build/build.sh
	@scripts/build/build.sh

run: all
	qemu-system-x86_64 -cdrom AurorOS.iso