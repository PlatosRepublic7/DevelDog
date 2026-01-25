#pragma once
#include "color.h"
#include <vector>

namespace dd {

struct Cell {
    char32_t content = ' ';
    Style style;

    // We should override the == operator for cell comparison
    bool operator==(const Cell &other) const {
        return content == other.content && style == other.style;
    }

    bool operator!=(const Cell &other) const { return !(*this == other); }
};

class Buffer {
  private:
    int m_width;
    int m_height;
    std::vector<Cell> m_cells;

  public:
    Buffer(int w, int h) : m_width(w), m_height(h), m_cells(w * h, Cell{}) {}

    // We will use a flat vector for performance: index = y * width + x
    int get_index(int x, int y) const { return (y * m_width) + x; }

    // Set a specific cell's data
    void set_cell(int x, int y, const Cell &cell) {
        if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
            m_cells[get_index(x, y)] = cell;
        }
    }

    // Retrieve a cell
    const Cell &get_cell(int x, int y) const { return m_cells[get_index(x, y)]; }

    // Reset the entire Buffer
    void clear() {
        for (auto &cell : m_cells) {
            cell.content = ' ';
            cell.style = Style{};
        }
    }

    // Resizing the Buffer
    void resize(int new_w, int new_h) {
        m_width = new_w;
        m_height = new_h;
        m_cells.assign(m_width * m_height, Cell{});
    }

    // Getters for dimensions
    int get_width() const { return m_width; }
    int get_height() const { return m_height; }
};
} // namespace dd
