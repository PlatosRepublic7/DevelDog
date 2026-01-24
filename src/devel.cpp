#include "devel.h"
#include "engine.h"
#include <termios.h>
#include <unistd.h>

DevelDog::DevelDog() { m_engine = std::make_unique<Engine>(); }

void DevelDog::move_cursor(int x, int y) { m_engine->move_cursor(x, y); }

void DevelDog::write(const std::string &text) { m_engine->write(text); }

void DevelDog::flush() { m_engine->flush(); }

void DevelDog::run() { m_engine->run(); }

void DevelDog::stop() { m_engine->stop(); }
