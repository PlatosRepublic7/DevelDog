#include "devel.h"
#include "renderer.h"
#include <fcntl.h>
#include <iostream>
#include <termios.h>
#include <unistd.h>

DevelDog::DevelDog() { m_renderer = std::make_unique<Renderer>(); }

void DevelDog::move_cursor(int x, int y) { m_renderer->move_cursor(x, y); }

void DevelDog::write(const std::string &text) { m_renderer->write(text); }

void DevelDog::flush() { m_renderer->flush(); }

void DevelDog::run() {

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

void DevelDog::stop() {
    // Exit alternate buffer and show cursor
    std::cout << "\e[?1049l";
    std::cout << "\e[?25h";
    std::cout.flush();

    // Restore original terminal settings
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
}
