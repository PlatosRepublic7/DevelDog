#include "buffer.h"
#include <assert.h>

namespace dd {
Buffer::Buffer(int w, int h) : m_width(w), m_height(h), m_cells(w * h, Cell{}) {}

// We will use a flat vector for performance: index = y * width + x
int Buffer::get_index(int x, int y) const { return (y * m_width) + x; }

// Set a specific cell's data
void Buffer::set_cell(int x, int y, const Cell &cell) noexcept {
    // In Release builds, this assertion will be deleted by the compiler
    // assert(x < 0 || x >= m_width || y < 0 || y > m_height);

    if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
        m_cells[get_index(x, y)] = cell;
    }
}

// Retrieve a cell
const Cell &Buffer::get_cell(int x, int y) const noexcept {
    static const Cell sentinal{' ', Style{}};

    // In Release builds, this assertion will be deleted by the compiler
    // so it is handy for developing
    // assert(x < 0 || x >= m_width || y < 0 || y >= m_height);

    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        return sentinal;
    }

    return m_cells[get_index(x, y)];
}

// Reset the entire Buffer
void Buffer::clear() {
    for (auto &cell : m_cells) {
        cell.content = ' ';
        cell.style = Style{};
    }
}

// Get the Buffer size
size_t Buffer::get_size() const { return m_cells.size(); }

// Resizing the Buffer
void Buffer::resize(int new_w, int new_h) {
    m_width = new_w;
    m_height = new_h;
    m_cells.assign(m_width * m_height, Cell{});
}

// Getters for dimensions
int Buffer::get_width() const { return m_width; }
int Buffer::get_height() const { return m_height; }
} // namespace dd
