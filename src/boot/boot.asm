; boot.asm
; Bootloader for AltOS

; Multiboot header constants
MULTIBOOT_HEADER_MAGIC  equ 0x1BADB002
MULTIBOOT_PAGE_ALIGN    equ 1<<0
MULTIBOOT_MEMORY_INFO   equ 1<<1
MULTIBOOT_FLAGS         equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO
MULTIBOOT_CHECKSUM      equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_FLAGS)

section .multiboot
align 4
    dd MULTIBOOT_HEADER_MAGIC   ; magic number
    dd MULTIBOOT_FLAGS          ; flags
    dd MULTIBOOT_CHECKSUM       ; checksum

section .bss
align 16
stack_bottom:
    resb 16384 ; 16 KiB
stack_top:

section .text
global _start
extern kernel_main

_start:
    ; Set up the stack
    mov esp, stack_top
    
    ; Push multiboot info pointer (passed in ebx)
    push ebx
    
    ; Call the kernel main function
    call kernel_main
    
    ; If the kernel returns, just hang
.hang:
    cli      ; Disable interrupts
    hlt      ; Halt the CPU
    jmp .hang ; Just in case