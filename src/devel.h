#pragma once
#include "renderer.h"
#include <memory>
#include <string>
#include <termios.h>

class DevelDog {
  public:
    DevelDog();

    void move_cursor(int x, int y);
    void write(const std::string &text);
    void flush();

    void run();
    void stop();

  private:
    std::unique_ptr<Renderer> m_renderer;
};
