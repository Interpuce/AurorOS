ROOT_DIR  := .

# directories
SRC_DIR           := $(ROOT_DIR)/src
BIN_DIR           := $(ROOT_DIR)/bin
ISO_DIR           := $(ROOT_DIR)/iso
BOOT_DIR          := $(ISO_DIR)/boot
GRUB_DIR          := $(BOOT_DIR)/grub
ARCHITECTURE_DIR  := $(SRC_DIR)/arch/x86_64

# files
KERNEL_BIN     := $(ROOT_DIR)/kernel.bin
ISO_FILE       := $(ROOT_DIR)/AurorOS.iso
LINKER_SCRIPT  := $(ARCHITECTURE_DIR)/build/linker.ld

# sources
C_SOURCES      := $(shell find $(SRC_DIR) -type f -name '*.c' ! -name '*.excluded.c')
CPP_SOURCES    := $(shell find $(SRC_DIR) -type f -name '*.cpp' ! -name '*.excluded.cpp')
ASM_SOURCES    := $(shell find $(SRC_DIR) -type f -name '*.asm')

# objects
C_OBJECTS      := $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%.o,$(C_SOURCES))
CPP_OBJECTS    := $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(CPP_SOURCES))
ASM_OBJECTS    := $(patsubst $(SRC_DIR)/%.asm,$(BIN_DIR)/%.o,$(ASM_SOURCES))

# all objects
OBJECTS        := $(C_OBJECTS) $(CPP_OBJECTS) $(ASM_OBJECTS)

# main target
all: build_kernel build_iso
	@echo -e "\033[32mSuccess!\033[0m"

# build c sources
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo -e "\033[1;36m[*]\033[0m $< -> $@"
	@gcc -g -Wall -Wextra -m64 -ffreestanding -nostartfiles -Iinclude -nostdlib -fno-stack-protector -c $< -o $@

# build c++ sources
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo -e "\033[1;36m[*]\033[0m $< -> $@"
	@g++ -g -Wall -Wextra -m64 -ffreestanding -fno-rtti -fno-threadsafe-statics -nostartfiles -Iinclude -nostdlib -fno-stack-protector -fno-exceptions -c $< -o $@

# build assembly sources
$(BIN_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	@echo -e "\033[1;36m[*]\033[0m $< -> $@"
	@nasm -f elf64 $< -o $@

# link the kernel
build_kernel: $(OBJECTS)
	@echo -e "\033[1;33m[*]\033[0m Linking objects -> kernel binary"
	@ld -m elf_x86_64 -T $(LINKER_SCRIPT) -o $(KERNEL_BIN) $(OBJECTS)

# build the iso
build_iso: build_kernel
	@echo -e "\033[1;33m[*]\033[0m Creating ISO directory structure"
	@mkdir -p $(GRUB_DIR)
	@cp $(KERNEL_BIN) $(BOOT_DIR)
	@cp $(ARCHITECTURE_DIR)/build/grub.cfg $(GRUB_DIR)/grub.cfg
	@echo -e "\033[1;33m[*]\033[0m Generating ISO with GRUB"
	@grub-mkrescue -o $(ISO_FILE) $(ISO_DIR)

# cleaning up
clean:
	@echo -e "\033[1;33m[*]\033[0m Cleaning..."
	@rm -rf $(BIN_DIR) $(ISO_DIR) $(KERNEL_BIN) $(ISO_FILE)

# running the iso
run: all
	qemu-system-x86_64 -cdrom AurorOS.iso

run_dbg: all
	@chmod +x scripts/run_debug_mode.sh
	./scripts/run_debug_mode.sh

# recompiling
recompile: clean all	
