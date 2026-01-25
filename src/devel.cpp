#include "devel.h"
#include "renderer.h"
#include <termios.h>
#include <unistd.h>

DevelDog::DevelDog() { m_renderer = std::make_unique<Renderer>(); }

void DevelDog::move_cursor(int x, int y) { m_renderer->move_cursor(x, y); }

void DevelDog::write(const std::string &text) { m_renderer->write(text); }

void DevelDog::flush() { m_renderer->flush(); }

void DevelDog::run() { m_renderer->run(); }

void DevelDog::stop() { m_renderer->stop(); }
