#pragma once
#include <string>

class Renderer {
  public:
    Renderer();

    void move_cursor(int x, int y);
    void write(const std::string &text);
    void flush();
};
