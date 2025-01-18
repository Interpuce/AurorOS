CC = clang
AS = nasm
LD = ld.lld

CFLAGS = --target=i686-elf -Wall -Wextra -m32 -ffreestanding -nostartfiles -Iinclude -nostdlib -fno-stack-protector -c
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T link.ld

KERNEL_SOURCES = kernel/kernel.c drivers/hardware/ports.c kernel/panic.c drivers/keyboard/input.c drivers/misc/memory.c drivers/vga/screen.c lib/msg.c lib/string.c apps/terminal/terminal.c apps/terminal/commands.c app/tinypad/tinypad.c drivers/hardware/cpu.c
KERNEL_OBJECTS = bin/kernel.o bin/ports.o bin/panic.o bin/input.o bin/memory.o bin/screen.o bin/msg.o bin/string.o bin/terminal.o bin/commands.o bin/tinypad.o bin/cpu.o
KERNEL_SOURCES_ASM = boot/boot.asm
KERNEL_OBJECTS_ASM = bin/boot.o

all: bin $(KERNEL_OBJECTS) $(KERNEL_OBJECTS_ASM) kernel.elf

bin:
	mkdir bin

kernel.elf: $(KERNEL_OBJECTS) $(KERNEL_OBJECTS_ASM)
	$(LD) $(LDFLAGS) $^ -o $@

$(KERNEL_OBJECTS_ASM): $(KERNEL_SOURCES_ASM)
	$(AS) $(ASFLAGS) -o $@ $^

bin/%.o: kernel/%.c
	$(CC) $(CFLAGS) -o $@ $<

bin/%.o: drivers/keyboard/%.c
	$(CC) $(CFLAGS) -o $@ $<

bin/%.o: drivers/misc/%.c
	$(CC) $(CFLAGS) -o $@ $<

bin/%.o: drivers/vga/%.c
	$(CC) $(CFLAGS) -o $@ $<

bin/%.o: drivers/hardware/%.c
	$(CC) $(CFLAGS) -o $@ $<
	
bin/%.o: lib/%.c
	$(CC) $(CFLAGS) -o $@ $<

bin/%.o: apps/terminal/%.c
	$(CC) $(CFLAGS) -o $@ $<

bin/%.o: apps/tinypad/%.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(KERNEL_OBJECTS) $(KERNEL_OBJECTS_ASM) kernel.elf

run:
	qemu-system-x86_64 -kernel kernel.elf
