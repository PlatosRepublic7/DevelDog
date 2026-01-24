#pragma once
#include <string>
#include <termios.h>

class Engine {
    struct termios original_termios;

  public:
    Engine();

    void run();
    void stop();
    void move_cursor(int x, int y);
    void write(const std::string &text);
    void flush();
};
