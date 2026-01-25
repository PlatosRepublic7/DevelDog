#include "renderer.h"
#include <iostream>

Renderer::Renderer() {}

void Renderer::move_cursor(int x, int y) { std::cout << "\e[" << y + 1 << ";" << x + 1 << "H"; }

void Renderer::write(const std::string &text) { std::cout << text; }

void Renderer::flush() { std::cout.flush(); }
