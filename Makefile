ROOT_DIR  := .

# architecture
ARCH ?= x86
SUPPORTED_ARCHES := x86 x86_64

ifeq ($(filter $(ARCH),$(SUPPORTED_ARCHES)),)
$(error Unsupported ARCH='$(ARCH)'. Supported values: $(SUPPORTED_ARCHES))
endif

ifeq ($(ARCH), x86)
    ARCH_M = 32
    ARCH_ELFFORMAT = i386
else 
    ARCH_M = 64
    ARCH_ELFFORMAT = x86_64
endif

# directories
SRC_DIR        := $(ROOT_DIR)/src
BIN_DIR        := $(ROOT_DIR)/bin/$(ARCH_ELFFORMAT)
ISO_DIR        := $(ROOT_DIR)/iso
BOOT_DIR       := $(ISO_DIR)/boot
GRUB_DIR       := $(BOOT_DIR)/grub
ARCH_DIR       := $(SRC_DIR)/arch/$(ARCH)

# files
KERNEL_BIN     := $(ROOT_DIR)/kernel.bin
ISO_FILE       := $(ROOT_DIR)/AurorOS.iso
LINKER_SCRIPT  := $(ARCH_DIR)/build/linker.ld
GRUB_CONFIG    := $(ARCH_DIR)/build/grub.cfg

# sources
C_SOURCES      := $(shell find $(SRC_DIR) -type f -name '*.c' ! -name '*.excluded.c')
CPP_SOURCES    := $(shell find $(SRC_DIR) -type f -name '*.cpp' ! -name '*.excluded.cpp')
ASM_SOURCES    := $(shell find $(SRC_DIR) -type f -name '*.asm')

# filtered sources
#  assuming C++ is only in src/apps, so there is no need
#  to exclude it based on architecture
C_SOURCES_FILTERED := \
    $(filter-out $(SRC_DIR)/arch/%,$(C_SOURCES)) \
    $(filter $(SRC_DIR)/arch/$(ARCH)/%,$(C_SOURCES))
ASM_SOURCES_FILTERED := \
    $(filter-out $(SRC_DIR)/arch/%,$(ASM_SOURCES)) \
    $(filter $(SRC_DIR)/arch/$(ARCH)/%,$(ASM_SOURCES)) 

# objects
C_OBJECTS      := $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%.o,$(C_SOURCES_FILTERED))
CPP_OBJECTS    := $(patsubst $(SRC_DIR)/%.cpp,$(BIN_DIR)/%.o,$(CPP_SOURCES))
ASM_OBJECTS    := $(patsubst $(SRC_DIR)/%.asm,$(BIN_DIR)/%.o,$(ASM_SOURCES_FILTERED))

# all objects
OBJECTS        := $(C_OBJECTS) $(CPP_OBJECTS) $(ASM_OBJECTS)

# main target
all: build_kernel build_iso
	@echo -e "\033[32mSuccess!\033[0m"

# build c sources
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo -e "\033[1;36m[*]\033[0m $< -> $@"
	@gcc -std=c99 -g -Wall -Wextra -m$(ARCH_M) -ffreestanding -nostartfiles -Iinclude -nostdlib -fno-stack-protector -c $< -o $@

# build c++ sources
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo -e "\033[1;36m[*]\033[0m $< -> $@"
	@g++ -g -Wall -Wextra -m$(ARCH_M) -ffreestanding -fno-rtti -fno-threadsafe-statics -nostartfiles -Iinclude -nostdlib -fno-stack-protector -fno-exceptions -c $< -o $@

# build assembly sources
$(BIN_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	@echo -e "\033[1;36m[*]\033[0m $< -> $@"
	@nasm -f elf$(ARCH_M) $< -o $@

# link the kernel
build_kernel: $(OBJECTS)
	@echo -e "\033[1;33m[*]\033[0m Linking objects -> kernel binary"
	@ld -m elf_$(ARCH_ELFFORMAT) -T $(LINKER_SCRIPT) -o $(KERNEL_BIN) $(OBJECTS)

# build the iso
build_iso: build_kernel
	@echo -e "\033[1;33m[*]\033[0m Creating ISO directory structure"
	@mkdir -p $(GRUB_DIR)
	@cp $(KERNEL_BIN) $(BOOT_DIR)
	@cp $(ARCH_DIR)/build/grub.cfg $(GRUB_DIR)/grub.cfg
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
	./scripts/run_debug_mode.sh $(ARCH_ELFFORMAT)

# recompile
recompile: clean all
