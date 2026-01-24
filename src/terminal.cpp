#include "terminal.h"
#include <fcntl.h> // For non-blocking
#include <iostream>
#include <termios.h>
#include <unistd.h>

Terminal::Terminal() {
    // Save original state
    tcgetattr(STDIN_FILENO, &original_termios);

    // Enter raw mode
    struct termios raw = original_termios;
    raw.c_lflag &= ~(ECHO | ICANON); // Turn off echo and line-by-line input
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    // Set STDIN to non-blocking
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    // ANSI escape sequences: Escape buffer and hide cursor
    std::cout << "\e[?1049h"; // Enter alt buffer
    std::cout << "\e[?25l";   // Hide cursor
    std::cout.flush();
}

Terminal::~Terminal() {
    // Exit alternate buffer and show cursor
    std::cout << "\e[?1049l";
    std::cout << "\e[?25h";
    std::cout.flush();

    // Restore original terminal settings
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
}

void Terminal::move_cursor(int x, int y) { std::cout << "\e[" << y + 1 << ";" << x + 1 << "H"; }

void Terminal::write(const std::string &text) { std::cout << text; }

void Terminal::flush() { std::cout.flush(); }
