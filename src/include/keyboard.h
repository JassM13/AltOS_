#ifndef ALTOS_KEYBOARD_H
#define ALTOS_KEYBOARD_H

#include <stdint.h>

namespace Keyboard {
    // Keyboard ports
    static const uint16_t DATA_PORT = 0x60;
    static const uint16_t STATUS_PORT = 0x64;

    // Special keys
    static const uint8_t KEY_BACKSPACE = 0x0E;
    static const uint8_t KEY_ENTER = 0x1C;
    static const uint8_t KEY_LEFT_SHIFT = 0x2A;
    static const uint8_t KEY_RIGHT_SHIFT = 0x36;
    static const uint8_t KEY_CAPS_LOCK = 0x3A;
    static const uint8_t KEY_UP = 0x48;
    static const uint8_t KEY_DOWN = 0x50;

    // Keyboard state
    extern bool shift_pressed;
    extern bool caps_lock_on;

    // Initialize keyboard
    void initialize();

    // Read from keyboard
    uint8_t read_scan_code();
    char scan_code_to_ascii(uint8_t scan_code);

    // Process keyboard input
    void handle_keypress();
}

#endif // ALTOS_KEYBOARD_H