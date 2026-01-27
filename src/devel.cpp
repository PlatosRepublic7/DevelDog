#include "devel.h"
#include <asm-generic/ioctls.h>
#include <fcntl.h>
#include <iostream>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

namespace dd {
DevelDog::DevelDog(bool debug_state)
    : m_debug_state(debug_state), m_is_running(false), m_width(0), m_height(0) {
    m_renderer = std::make_unique<Renderer>();
}

void DevelDog::init_terminal() {
    // Get current window size using ioctl
    get_window_dimensions();
    m_back_buffer = std::make_unique<Buffer>(m_width, m_height);

    // TTY Settings
    tcgetattr(STDIN_FILENO, &m_original_settings);
    struct termios raw = m_original_settings;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    // Non-blocking input
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);

    std::string startup_codes = "\e[?1049h\e[?25l\e[?7l";
    // Hardware switch: Alt buffer, hide cursor, clear
    ::write(STDOUT_FILENO, startup_codes.data(), startup_codes.size());
}

void DevelDog::get_window_dimensions() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    m_width = w.ws_col;
    m_height = w.ws_row;
}

bool DevelDog::update_buffer_dimensions() {
    if (m_width != m_back_buffer->get_width() || m_height != m_back_buffer->get_height()) {
        return true;
    }
    return false;
}

void DevelDog::start() {
    if (m_is_running)
        return;

    init_terminal();
    m_is_running = true;
    main_loop();
    stop();
}

void DevelDog::main_loop() {
    while (m_is_running) {
        // Marshaller Coordination
        // Input check
        char c;
        if (read(STDIN_FILENO, &c, 1) == 1 && c == 'q') {
            break;
        }

        // Get current window dimensions
        get_window_dimensions();

        // Check to see if we need to update the buffer dimensions
        // if so, update them, and force the terminal to clear the scroll buffer
        if (update_buffer_dimensions()) {
            m_back_buffer->resize(m_width, m_height);

            // Immediate clear to resync with hardware
            // ::write(STDOUT_FILENO, "\e[3J", 4);
        }

        // Clear the buffer contents in preparation for drawing
        m_back_buffer->clear();

        // Draw all components
        for (auto &comp : m_components) {
            comp->draw(*m_back_buffer);
        }

        // Render compilation
        std::string frame = m_renderer->render(*m_back_buffer, m_debug_state);

        // Write to terminal output and sleep
        ::write(STDOUT_FILENO, frame.data(), frame.size());

        usleep(16666);
    }
}

void DevelDog::stop() {
    if (!m_is_running)
        return;
    m_is_running = false;

    // Reverse the hardware switch
    std::string stop_codes = "\e[?1049l\e[?25h\e[?7h";
    ::write(STDOUT_FILENO, stop_codes.data(), stop_codes.size());
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &m_original_settings);
}

DevelDog::~DevelDog() {
    // We safety check if the user forgets to call stop()
    if (m_is_running)
        stop();
}

} // namespace dd
