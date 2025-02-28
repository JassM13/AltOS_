#!/bin/bash
# build.sh - Build script for AltOS

# Exit on error
set -e

# Create directories if they don't exist
mkdir -p build

# Compile boot.asm
echo "Assembling bootloader..."
nasm -f elf32 src/boot/boot.asm -o build/boot.o

# Compile kernel
echo "Compiling kernel..."
i686-elf-g++ -c src/kernel/kernel.cpp -o build/kernel.o -ffreestanding -O2 -Wall -Wextra -fno-exceptions -fno-rtti

# Link the kernel
echo "Linking kernel..."
i686-elf-g++ -T src/kernel/linker.ld -o build/kernel.bin -ffreestanding -O2 -nostdlib build/boot.o build/kernel.o -lgcc

echo "Build complete! Kernel binary is at build/kernel.bin"
echo "Run with: ./scripts/run.sh"