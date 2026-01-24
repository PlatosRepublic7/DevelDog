#pragma once
#include <string>
#include <termios.h>

class DevelDog {
    struct termios original_termios;

  public:
    DevelDog();
    ~DevelDog();
    void move_cursor(int x, int y);
    void write(const std::string &text);
    void flush();
};
