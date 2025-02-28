#!/bin/bash
# run.sh - Run script for AltOS

# Build the OS first
echo "Building AltOS..."
./scripts/build.sh

# Check if build was successful
if [ ! -f build/kernel.bin ]; then
    echo "Error: Build failed. Kernel binary not found."
    exit 1
fi

# Run QEMU with the kernel binary
echo "Starting AltOS in QEMU..."
qemu-system-i386 -kernel build/kernel.bin