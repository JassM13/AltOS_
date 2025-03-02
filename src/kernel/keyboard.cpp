#include "../include/keyboard.h"
#include "../include/terminal.h"

namespace Keyboard {
    bool shift_pressed = false;
    bool caps_lock_on = false;

    // Scan code to ASCII lookup table (US QWERTY layout)
    static const char ascii_table[] = {
        0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,
        0, 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0,
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
        0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
        '*', 0, ' '
    };

    static const char ascii_table_shift[] = {
        0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0,
        0, 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 0,
        0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~',
        0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
        '*', 0, ' '
    };

    // Helper function to read from I/O port
    static inline uint8_t inb(uint16_t port) {
        uint8_t value;
        asm volatile ("inb %1, %0" : "=a"(value) : "Nd"(port));
        return value;
    }

    void initialize() {
        shift_pressed = false;
        caps_lock_on = false;
    }

    uint8_t read_scan_code() {
        uint8_t scan_code;
        // Wait for keyboard buffer to be ready
        while ((inb(STATUS_PORT) & 0x01) == 0);
        scan_code = inb(DATA_PORT);
        return scan_code;
    }

    char scan_code_to_ascii(uint8_t scan_code) {
        if (scan_code >= sizeof(ascii_table)) return 0;

        char c = 0;
        bool is_alpha = (ascii_table[scan_code] >= 'a' && ascii_table[scan_code] <= 'z');
        bool should_capitalize = (shift_pressed != caps_lock_on);

        if (shift_pressed) {
            c = ascii_table_shift[scan_code];
        } else {
            c = ascii_table[scan_code];
        }

        if (is_alpha && should_capitalize) {
            c = ascii_table_shift[scan_code];
        }
        return c;
    }

    void handle_keypress() {
        uint8_t scan_code = read_scan_code();
        
        // Handle key release
        if (scan_code & 0x80) {
            scan_code &= 0x7F;
            if (scan_code == KEY_LEFT_SHIFT || scan_code == KEY_RIGHT_SHIFT) {
                shift_pressed = false;
            }
            return;
        }

        // Handle special keys
        switch (scan_code) {
            case KEY_BACKSPACE:
                Terminal::handle_backspace();
                break;

            case KEY_ENTER:
                Terminal::handle_enter();
                break;

            case KEY_LEFT_SHIFT:
            case KEY_RIGHT_SHIFT:
                shift_pressed = true;
                break;

            case KEY_CAPS_LOCK:
                caps_lock_on = !caps_lock_on;
                break;

            default:
                char c = scan_code_to_ascii(scan_code);
                if (c != 0) {
                    Terminal::handle_character(c);
                }
                break;
        }
    }
}