#include "../include/terminal.h"
#include <stdint.h>
#include <stddef.h>

namespace Terminal {
    // Forward declarations
    int strcmp(const char* s1, const char* s2);

    // Terminal state variables
    uint16_t cursor_x = 0;
    uint16_t cursor_y = 0;
    char command_buffer[MAX_COMMAND_LENGTH];
    size_t command_length = 0;

    // Command history
    static char command_history[COMMAND_HISTORY_SIZE][MAX_COMMAND_LENGTH];
    static size_t history_count = 0;
    static size_t history_index = 0;

    void initialize() {
        cursor_x = 0;
        cursor_y = 0;
        command_length = 0;
        history_count = 0;
        history_index = 0;
        Graphics::clear();
        print_prompt();
    }

    void print_prompt() {
        Graphics::set_color(static_cast<Graphics::Color>(2), static_cast<Graphics::Color>(0));
        Graphics::put_string("AltOS> ", cursor_x, cursor_y);
        cursor_x += 7; // Length of "AltOS> "
        Graphics::set_color(static_cast<Graphics::Color>(15), static_cast<Graphics::Color>(0));
        Graphics::set_cursor_pos(cursor_x, cursor_y);
    }

    void handle_backspace() {
        if (command_length > 0) {
            command_length--;
            if (cursor_x > 0) {
                cursor_x--;
                Graphics::put_char(' ', cursor_x, cursor_y);
                Graphics::set_cursor_pos(cursor_x, cursor_y);
            }
        }
    }

    void handle_enter() {
        command_buffer[command_length] = '\0';
        cursor_x = 0;
        cursor_y++;
        
        if (command_length > 0) {
            add_to_history(command_buffer);
            execute_command();
        }

        if (cursor_y >= Graphics::VGA_HEIGHT) {
            Graphics::scroll();
            cursor_y--;
        }

        command_length = 0;
        print_prompt();
    }

    void handle_character(char c) {
        if (command_length < MAX_COMMAND_LENGTH - 1) {
            command_buffer[command_length++] = c;
            Graphics::put_char(c, cursor_x++, cursor_y);
            if (cursor_x >= Graphics::VGA_WIDTH) {
                cursor_x = 0;
                cursor_y++;
                if (cursor_y >= Graphics::VGA_HEIGHT) {
                    Graphics::scroll();
                    cursor_y--;
                }
            }
            Graphics::set_cursor_pos(cursor_x, cursor_y);
        }
    }

    void add_to_history(const char* command) {
        if (history_count < COMMAND_HISTORY_SIZE) {
            for (size_t i = 0; i < MAX_COMMAND_LENGTH; i++) {
                command_history[history_count][i] = command[i];
                if (command[i] == '\0') break;
            }
            history_count++;
        } else {
            // Shift history entries down
            for (size_t i = 0; i < COMMAND_HISTORY_SIZE - 1; i++) {
                for (size_t j = 0; j < MAX_COMMAND_LENGTH; j++) {
                    command_history[i][j] = command_history[i + 1][j];
                }
            }
            // Add new command at the end
            for (size_t i = 0; i < MAX_COMMAND_LENGTH; i++) {
                command_history[COMMAND_HISTORY_SIZE - 1][i] = command[i];
                if (command[i] == '\0') break;
            }
        }
        history_index = history_count;
    }

    const char* get_previous_command() {
        if (history_count > 0 && history_index > 0) {
            history_index--;
            return command_history[history_index];
        }
        return nullptr;
    }

    const char* get_next_command() {
        if (history_index < history_count - 1) {
            history_index++;
            return command_history[history_index];
        }
        return nullptr;
    }

    void execute_command() {
        // Basic command processing
        if (command_length == 0) return;

        // Compare command with known commands
        if (strcmp(command_buffer, "clear") == 0) {
            Graphics::clear();
            cursor_x = 0;
            cursor_y = 0;
        }
        else if (strcmp(command_buffer, "help") == 0) {
            cursor_x = 0;
            cursor_y++;
            Graphics::put_string("Available commands:\n", 0, cursor_y++);
            Graphics::put_string("  clear - Clear the screen\n", 0, cursor_y++);
            Graphics::put_string("  help  - Show this help message\n", 0, cursor_y++);
        }
        else {
            cursor_x = 0;
            cursor_y++;
            Graphics::put_string("Unknown command: ", 0, cursor_y);
            Graphics::put_string(command_buffer, 16, cursor_y);
            cursor_y++;
        }
    }

    // Simple string comparison function
    int strcmp(const char* s1, const char* s2) {
        while (*s1 && (*s1 == *s2)) {
            s1++;
            s2++;
        }
        return *(const unsigned char*)s1 - *(const unsigned char*)s2;
    }
}