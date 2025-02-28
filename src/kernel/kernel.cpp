// kernel.cpp
// Main kernel file for AltOS

#include <stdint.h>
#include <stddef.h>

// VGA text mode constants
enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

// VGA text buffer address
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

// Screen dimensions
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

// Current cursor position
size_t terminal_row = 0;
size_t terminal_column = 0;
uint8_t terminal_color = 0;

// Create a VGA entry from character and color
uint16_t vga_entry(unsigned char c, uint8_t color) {
    return (uint16_t) c | (uint16_t) color << 8;
}

// Create a color attribute from foreground and background colors
uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

// Initialize the terminal
void terminal_initialize() {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    
    // Clear the screen
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            VGA_MEMORY[index] = vga_entry(' ', terminal_color);
        }
    }
}

// Set a character at a specific position
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    VGA_MEMORY[index] = vga_entry(c, color);
}

// Write a character to the terminal
void terminal_putchar(char c) {
    // Handle newline
    if (c == '\n') {
        terminal_column = 0;
        terminal_row++;
        if (terminal_row == VGA_HEIGHT) {
            terminal_row = 0; // Simple wrap-around for now
        }
        return;
    }
    
    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    
    // Advance cursor position
    terminal_column++;
    if (terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        terminal_row++;
        if (terminal_row == VGA_HEIGHT) {
            terminal_row = 0; // Simple wrap-around for now
        }
    }
}

// Write a string to the terminal
void terminal_writestring(const char* data) {
    for (size_t i = 0; data[i] != '\0'; i++) {
        terminal_putchar(data[i]);
    }
}

// Kernel entry point
extern "C" void kernel_main(void* multiboot_info) {
    // Initialize terminal
    terminal_initialize();
    
    // Welcome message
    terminal_writestring("Welcome to AltOS!\n");
    terminal_writestring("A simple operating system from scratch.\n");
    terminal_writestring("--------------------------------\n");
    
    // Hang forever
    while (1) {}
}