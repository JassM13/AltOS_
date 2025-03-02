#ifndef ALTOS_GRAPHICS_H
#define ALTOS_GRAPHICS_H

#include <stdint.h>
#include <stddef.h>

namespace Graphics {
    // Display constants
    static const uint16_t VGA_WIDTH = 80;
    static const uint16_t VGA_HEIGHT = 25;
    static const uintptr_t VGA_MEMORY = 0xB8000;
    
    // Color constants
    enum class Color : uint8_t {
        BLACK = 0,
        BLUE = 1,
        GREEN = 2,
        CYAN = 3,
        RED = 4,
        MAGENTA = 5,
        BROWN = 6,
        LIGHT_GREY = 7,
        DARK_GREY = 8,
        LIGHT_BLUE = 9,
        LIGHT_GREEN = 10,
        LIGHT_CYAN = 11,
        LIGHT_RED = 12,
        LIGHT_MAGENTA = 13,
        YELLOW = 14,
        WHITE = 15
    };
    
    // Display control
    void initialize();
    void clear();
    void set_color(Color foreground, Color background);
    
    // Basic drawing
    void put_char(char c, uint16_t x, uint16_t y);
    void put_string(const char* str, uint16_t x, uint16_t y);
    void draw_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, char c);
    
    // Cursor control
    void set_cursor_pos(uint16_t x, uint16_t y);
    void enable_cursor();
    void disable_cursor();
    
    // Screen buffer management
    void scroll();
    void update_screen();
}

#endif // ALTOS_GRAPHICS_H