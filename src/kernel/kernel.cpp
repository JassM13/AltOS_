// kernel.cpp
// Main kernel file for AltOS

#include "../include/graphics.h"
#include "../include/terminal.h"
#include "../include/keyboard.h"

// Kernel entry point
extern "C" void kernel_main(void* multiboot_info) {
    // Initialize graphics system
    Graphics::initialize();
    Graphics::set_color(Graphics::Color::LIGHT_GREY, Graphics::Color::BLACK);
    
    // Welcome message
    Graphics::put_string("Welcome to AltOS!\n", 0, 0);
    Graphics::put_string("Type 'help' for available commands.\n", 0, 1);
    Graphics::put_string("--------------------------------\n", 0, 2);
    
    // Initialize terminal
    Terminal::initialize();
    
    // Initialize keyboard
    Keyboard::initialize();
    
    // Main kernel loop
    while (1) {
        // Handle keyboard input
        Keyboard::handle_keypress();
    }
}