#include "renderer.h"
#include <variant>

namespace dd {
Renderer::Renderer() : m_cursor_line_pos(0), m_cursor_row_pos(0) {};

std::vector<int> Renderer::get_cursor_loc() {
    std::vector<int> cursor_loc = {m_cursor_row_pos, m_cursor_line_pos};
    return cursor_loc;
}

std::string Renderer::render(const Buffer &back, const Buffer &front) {
    std::string output;
    Style last_style;
    bool cursor_needs_move = false;

    int width = back.get_width();
    int height = back.get_height();

    for (int y = 0; y < height; ++y) {
        cursor_needs_move = true;
        for (int x = 0; x < width; ++x) {
            const Cell &new_cell = back.get_cell(x, y);
            const Cell &old_cell = front.get_cell(x, y);
            Cell cell_to_draw = {};

            if (new_cell != old_cell) {
                // If we aren't already at this position, move the cursor to this position
                if (cursor_needs_move) {
                    output += move_cursor(x, y);
                    cursor_needs_move = false;
                }

                // Apply style (only if different from the last one)
                if (new_cell.style != last_style) {
                    output += format_style(new_cell.style);
                    last_style = new_cell.style;
                }

                // Add the character
                output += new_cell.content;
            } else {
                cursor_needs_move = true;
            }
        }
    }
    return output;
}

std::string Renderer::format_style(const Style &style) {
    // Start with a reset to ensure no style bleed from previous calls
    std::string ansi = "\e[0m";

    // Foreground logic
    std::visit(
        [&ansi](auto &&arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, ColorName>) {
                if (arg != ColorName::Default) {
                    // ANSI foreground colors 30-37, 90-97
                    int code = 30 + static_cast<int>(arg) - 1;
                    ansi += "\e[" + std::to_string(code) + "m";
                }
            } else if constexpr (std::is_same_v<T, RGB>) {
                ansi += "\e[38;2;" + std::to_string(arg.r) + ";" + std::to_string(arg.g) + ";" +
                        std::to_string(arg.b) + "m";
            }
        },
        style.fg);

    // Background logic
    std::visit(
        [&ansi](auto &&arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, ColorName>) {
                if (arg != ColorName::Default) {
                    // ANSI background colors 40-47, 100-107
                    int code = 40 + static_cast<int>(arg) - 1;
                    ansi += "\e[" + std::to_string(code) + "m";
                }
            } else if constexpr (std::is_same_v<T, RGB>) {
                ansi += "\e[48;2;" + std::to_string(arg.r) + ";" + std::to_string(arg.g) + ";" +
                        std::to_string(arg.b) + "m";
            }
        },
        style.bg);

    // Attribute logic
    if (style.attributes & static_cast<uint8_t>(Attribute::Bold)) {
        ansi += "\e[1m";
    }

    return ansi;
}

std::string Renderer::move_cursor(int x, int y) {
    std::string move_code;
    move_code = "\e[" + std::to_string(y + 1) + ";" + std::to_string(x + 1) + "H";
    m_cursor_row_pos = x;
    m_cursor_line_pos = y;
    return move_code;
}

} // namespace dd
