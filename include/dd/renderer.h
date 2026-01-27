#pragma once
#include "buffer.h"
#include <string>

namespace dd {
class Renderer {
  public:
    Renderer();

    // Compile the entire buffer into one ANSI-encoded string
    std::string render(const Buffer &back, const Buffer &front);

    std::vector<int> get_cursor_loc();

  private:
    int m_RESERVE_CONST = 5;
    int m_cursor_row_pos;
    int m_cursor_line_pos;

    // Helper to turn Style struct into ANSI escape codes
    std::string format_style(const Style &style);

    std::string move_cursor(int x, int y);
};
} // namespace dd
