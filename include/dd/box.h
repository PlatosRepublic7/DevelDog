#pragma once
#include "component.h"
#include "theme.h"

namespace dd {
class Box : public Component {
  public:
    Box(int x, int y, int width, int height, Style style)
        : m_x(x), m_y(y), m_width(width), m_height(height), m_style(style) {}

    void draw(Buffer &buf) override {
        // Draw Horizontal lines
        for (int i = 1; i < m_width; ++i) {
            buf.set_cell(m_x + i, m_y, {Border::Single_H, m_style});
            buf.set_cell(m_x + i, m_y + m_height - 1, {Border::Single_H, m_style});
        }

        // Draw Vertical lines
        for (int i = 1; i < m_height; ++i) {
            buf.set_cell(m_x, m_y + i, {Border::Single_V, m_style});
            buf.set_cell(m_x + m_width - 1, m_y + i, {Border::Single_V, m_style});
        }

        // Draw Corners
        buf.set_cell(m_x, m_y, {Border::Single_TL, m_style});
        buf.set_cell(m_x + m_width - 1, m_y, {Border::Single_TR, m_style});
        buf.set_cell(m_x, m_y + m_height - 1, {Border::Single_BL, m_style});
        buf.set_cell(m_x + m_width - 1, m_y + m_height - 1, {Border::Single_BR, m_style});
    }

  private:
    int m_x, m_y, m_width, m_height;
    Style m_style;
};
} // namespace dd
