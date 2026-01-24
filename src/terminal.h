#pragma once
#include <string>
#include <termios.h>

class Terminal {
    struct termios original_termios;

  public:
    Terminal();
    ~Terminal();
    void move_cursor(int x, int y);
    void write(const std::string &text);
    void flush();
};
