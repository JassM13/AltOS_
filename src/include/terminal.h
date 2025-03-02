#ifndef ALTOS_TERMINAL_H
#define ALTOS_TERMINAL_H

#include <stdint.h>
#include <stddef.h>
#include "graphics.h"

namespace Terminal {
    // Constants
    static const size_t MAX_COMMAND_LENGTH = 256;
    static const size_t COMMAND_HISTORY_SIZE = 10;

    // Terminal state
    extern uint16_t cursor_x;
    extern uint16_t cursor_y;
    extern char command_buffer[MAX_COMMAND_LENGTH];
    extern size_t command_length;

    // Terminal control
    void initialize();
    void process_keypress(uint8_t scancode);
    void execute_command();
    void print_prompt();

    // Input handling
    void handle_backspace();
    void handle_enter();
    void handle_character(char c);

    // Command history
    void add_to_history(const char* command);
    const char* get_previous_command();
    const char* get_next_command();
}

#endif // ALTOS_TERMINAL_H