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
    Buffer(int w, int h);

    // We will use a flat vector for performance: index = y * width + x
    int get_index(int x, int y) const;

    // Set a specific cell's data
    void set_cell(int x, int y, const Cell &cell) noexcept;

    // Retrieve a cell
    const Cell &get_cell(int x, int y) const noexcept;

    // Reset the entire Buffer
    void clear();

    size_t get_size() const;

    // Resizing the Buffer
    void resize(int new_w, int new_h);

    // Getters for dimensions
    int get_width() const;
    int get_height() const;
};
} // namespace dd
