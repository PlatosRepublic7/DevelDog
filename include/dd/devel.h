#pragma once
#include "buffer.h"
#include "renderer.h"
#include <memory>
#include <termios.h>

namespace dd {
class DevelDog {
  public:
    DevelDog();
    ~DevelDog();

    // Prevent copying
    DevelDog(const DevelDog &) = delete;
    DevelDog &operator=(const DevelDog &) = delete;

    // Lifecycle
    void start();
    void stop();

    Buffer &get_buffer();

    // Future: void attach(std::unique_ptr<Component> component);

  private:
    void init_terminal();
    void restore_terminal();
    void main_loop();
    void update_dimensions();

    bool m_is_running;
    struct termios m_original_settings;

    int m_width;
    int m_height;

    std::unique_ptr<Buffer> m_back_buffer;
    std::unique_ptr<Renderer> m_renderer;
};
} // namespace dd
