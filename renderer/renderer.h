#pragma once
#include <string>
#include <termios.h>

class Renderer {
    struct termios original_termios;

  public:
    Renderer();

    void run();
    void stop();
    void move_cursor(int x, int y);
    void write(const std::string &text);
    void flush();
};
