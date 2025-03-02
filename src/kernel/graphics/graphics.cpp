#include "../../include/graphics.h"
#include <stdint.h>
#include <stddef.h>

namespace Graphics {
    // VGA buffer pointer
    static uint16_t* vga_buffer = reinterpret_cast<uint16_t*>(VGA_MEMORY);
    static uint8_t current_color = 0x0F; // White on black by default
    
    void initialize() {
        clear();
        enable_cursor();
    }
    
    void clear() {
        for (uint16_t i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
            vga_buffer[i] = (current_color << 8) | ' ';
        }
        set_cursor_pos(0, 0);
    }
    
    void set_color(Color foreground, Color background) {
        current_color = (static_cast<uint8_t>(background) << 4) | static_cast<uint8_t>(foreground);
    }
    
    void put_char(char c, uint16_t x, uint16_t y) {
        if (x >= VGA_WIDTH || y >= VGA_HEIGHT) return;
        
        const size_t index = y * VGA_WIDTH + x;
        vga_buffer[index] = (current_color << 8) | c;
    }
    
    void put_string(const char* str, uint16_t x, uint16_t y) {
        for (size_t i = 0; str[i] != '\0'; i++) {
            if (x >= VGA_WIDTH) {
                x = 0;
                y++;
            }
            if (y >= VGA_HEIGHT) {
                scroll();
                y--;
            }
            put_char(str[i], x++, y);
        }
    }
    
    void draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, char c) {
        for (uint16_t i = 0; i < height; i++) {
            for (uint16_t j = 0; j < width; j++) {
                put_char(c, x + j, y + i);
            }
        }
    }
    
    void set_cursor_pos(uint16_t x, uint16_t y) {
        uint16_t pos = y * VGA_WIDTH + x;
        
        // Send command to VGA control ports
        asm volatile ("outb %%al, %%dx" : : "a"(0x0F), "d"(0x3D4));
        asm volatile ("outb %%al, %%dx" : : "a"((uint8_t)(pos & 0xFF)), "d"(0x3D5));
        asm volatile ("outb %%al, %%dx" : : "a"(0x0E), "d"(0x3D4));
        asm volatile ("outb %%al, %%dx" : : "a"((uint8_t)((pos >> 8) & 0xFF)), "d"(0x3D5));
    }
    
    void enable_cursor() {
        // Set cursor start and end lines
        asm volatile ("outb %%al, %%dx" : : "a"(0x0A), "d"(0x3D4));
        asm volatile ("outb %%al, %%dx" : : "a"(0x0), "d"(0x3D5));
        asm volatile ("outb %%al, %%dx" : : "a"(0x0B), "d"(0x3D4));
        asm volatile ("outb %%al, %%dx" : : "a"(15), "d"(0x3D5));
    }
    
    void disable_cursor() {
        asm volatile ("outb %%al, %%dx" : : "a"(0x0A), "d"(0x3D4));
        asm volatile ("outb %%al, %%dx" : : "a"(0x20), "d"(0x3D5));
    }
    
    void scroll() {
        // Move all lines up by one
        for (size_t i = 0; i < VGA_HEIGHT - 1; i++) {
            for (size_t j = 0; j < VGA_WIDTH; j++) {
                vga_buffer[i * VGA_WIDTH + j] = vga_buffer[(i + 1) * VGA_WIDTH + j];
            }
        }
        
        // Clear the last line
        for (size_t j = 0; j < VGA_WIDTH; j++) {
            vga_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + j] = (current_color << 8) | ' ';
        }
    }
    
    void update_screen() {
        // In text mode, screen updates are immediate
        // This function is kept for compatibility with future graphics modes
    }
}