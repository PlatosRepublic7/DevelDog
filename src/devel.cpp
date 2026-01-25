#include "devel.h"
#include <asm-generic/ioctls.h>
#include <fcntl.h>
#include <iostream>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

namespace dd {
DevelDog::DevelDog() : m_is_running(false), m_width(0), m_height(0) {
    m_renderer = std::make_unique<Renderer>();
    m_back_buffer = std::make_unique<Buffer>(m_width, m_height);
}

void DevelDog::init_terminal() {
    // Get current window size using ioctl
    update_dimensions();
    m_back_buffer = std::make_unique<Buffer>(m_width, m_height);

    // TTY Settings
    tcgetattr(STDIN_FILENO, &m_original_settings);
    struct termios raw = m_original_settings;
    raw.c_lflag &= ~(ECHO | ICANON);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    // Non-blocking input
    fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL) | O_NONBLOCK);

    // Hardware switch: Alt buffer, hide cursor, clear
    std::cout << "\e[?1049h\e[?25l";
    std::cout.flush();
}

void DevelDog::update_dimensions() {
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    m_width = w.ws_col;
    m_height = w.ws_row;
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

        Style test_style;
        test_style.fg = ColorName::Red;
        m_back_buffer->set_cell(5, 5, {'!', test_style});

        // Render compilation
        std::string frame = m_renderer->render(*m_back_buffer);

        // Output to terminal
        // std::cout << frame;
        // std::cout.flush();

        ::write(STDOUT_FILENO, frame.data(), frame.size());

        usleep(16666);
    }
}

void DevelDog::stop() {
    if (!m_is_running)
        return;
    m_is_running = false;

    // Reverse the hardware switch
    std::cout << "\e[?1049l\e[?25h";
    std::cout.flush();
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &m_original_settings);
}

DevelDog::~DevelDog() {
    // We safety check if the user forgets to call stop()
    if (m_is_running)
        stop();
}

Buffer &DevelDog::get_buffer() { return *m_back_buffer; }

} // namespace dd
