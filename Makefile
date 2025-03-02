# AltOS Makefile

# Cross-compiler settings
CROSS_COMPILE := i686-elf-
CC := $(CROSS_COMPILE)gcc
CXX := $(CROSS_COMPILE)g++
LD := $(CROSS_COMPILE)ld
ASM := nasm

# Directories
SRC_DIR := src
BUILD_DIR := build

# Find all source files
ASM_SRCS := $(shell find $(SRC_DIR) -name '*.asm')
CPP_SRCS := $(shell find $(SRC_DIR) -name '*.cpp')

# Generate object file names
ASM_OBJS := $(patsubst $(SRC_DIR)/%.asm,$(BUILD_DIR)/%.o,$(ASM_SRCS))
CPP_OBJS := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(CPP_SRCS))
OBJS := $(ASM_OBJS) $(CPP_OBJS)

# Flags
CFLAGS := -ffreestanding -O2 -Wall -Wextra -I$(SRC_DIR)/include
CXXFLAGS := $(CFLAGS) -fno-exceptions -fno-rtti
LDFLAGS := -ffreestanding -O2 -nostdlib
ASMFLAGS := -f elf32

# QEMU settings
QEMU := qemu-system-i386
QEMU_FLAGS := -kernel $(BUILD_DIR)/kernel.bin \
              -m 128M \
              -serial stdio

# Default target
.PHONY: all clean run
all: $(BUILD_DIR)/kernel.bin

# Create build directories
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	@$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm
	@mkdir -p $(dir $@)
	@echo "Assembling $<..."
	@$(ASM) $(ASMFLAGS) $< -o $@

# Link the kernel
$(BUILD_DIR)/kernel.bin: $(OBJS)
	@echo "Linking kernel..."
	@$(CXX) -T $(SRC_DIR)/kernel/linker.ld -o $@ $(LDFLAGS) $(OBJS) -lgcc
	@echo "Build complete! Binary is at $@"

# Run the kernel in QEMU
run: all
	@echo "Starting QEMU..."
	@$(QEMU) $(QEMU_FLAGS)

# Clean build files
clean:
	@echo "Cleaning build directory..."
	@rm -rf $(BUILD_DIR)

# Include dependency files
-include $(CPP_OBJS:.o=.d)

# Generate dependency files
$(BUILD_DIR)/%.d: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -MM -MT "$(@:.d=.o) $@" $< > $@