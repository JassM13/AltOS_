# AltOS

A simple operating system from scratch written in C++.

## Project Structure

```
/
├── src/                  # Source code
│   ├── boot/             # Bootloader code
│   ├── kernel/           # Kernel code
│   ├── drivers/          # Hardware drivers
│   └── include/          # Header files
├── build/                # Build output directory
├── iso/                  # ISO image files
├── scripts/              # Build and utility scripts
└── docs/                 # Documentation
```

## Requirements

- GCC cross-compiler for i686-elf
- NASM assembler
- GRUB bootloader
- xorriso (for ISO creation)
- QEMU (for testing)

## Building

To build the operating system:

```bash
./scripts/build.sh
```

This will compile the source code and generate an ISO image in the `build` directory.

## Running

To run the operating system in QEMU:

```bash
./scripts/run.sh
```

Or manually:

```bash
qemu-system-i386 -cdrom build/altos.iso
```

## Features

- Basic bootloader
- Simple kernel
- Text-mode display
- Keyboard input

## License

MIT License