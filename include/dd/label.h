#pragma once
#include "component.h"
#include <string>

namespace dd {
class Label : public Component {
  public:
    Label(int x, int y, std::string text, Style style)
        : m_x(x), m_y(y), m_text(text), m_style(style) {}

    void draw(Buffer &buf) override {
        for (size_t i = 0; i < m_text.length(); ++i) {
            buf.set_cell(m_x + i, m_y, {(char32_t)m_text[i], m_style});
        }
    }

  private:
    int m_x, m_y;
    std::string m_text;
    Style m_style;
};
} // namespace dd
