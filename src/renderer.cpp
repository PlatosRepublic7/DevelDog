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
                output += to_utf8(new_cell.content);
            } else {
                cursor_needs_move = true;
            }
        }
    }
    return output;
}

std::string Renderer::format_style(const Style &style) {
    // We use a single 'm' sequence to combine attributes for efficiency
    // Example: \e[0;31;44;1m (Reset, Red FG, Blue BG, Bold)
    std::string ansi = "\e[";

    // Foreground
    std::visit(
        [&ansi](auto &&arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, ColorName>) {
                if (arg != ColorName::Default) {
                    int code = 30 + static_cast<int>(arg) - 1;
                    ansi += ";" + std::to_string(code);
                }
            } else if constexpr (std::is_same_v<T, RGB>) {
                ansi += "38;2;" + std::to_string(arg.r) + ";" + std::to_string(arg.g) + ";" +
                        std::to_string(arg.b);
            }
        },
        style.fg);

    // Background
    std::visit(
        [&ansi](auto &&arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, ColorName>) {
                if (arg != ColorName::Default) {
                    int code = 40 + static_cast<int>(arg) - 1;
                    ansi += ";" + std::to_string(code);
                } else {
                    // 49 is ANSII default background
                    ansi += ";49";
                }
            } else if constexpr (std::is_same_v<T, RGB>) {
                ansi += "48;2;" + std::to_string(arg.r) + ";" + std::to_string(arg.g) + ";" +
                        std::to_string(arg.b);
            }
        },
        style.bg);

    // Attributes
    if (style.attributes & static_cast<uint8_t>(Attribute::Bold)) {
        ansi += ";1";
    }

    ansi += "m";
    return ansi;
}

std::string Renderer::to_utf8(char32_t cp) {
    if (cp == '0')
        return " ";
    std::string result;
    if (cp <= 0x7F) {
        result += static_cast<char>(cp);
    } else if (cp <= 0x7FF) {
        result += static_cast<char>(0xC0 | (cp >> 6));
        result += static_cast<char>(0x80 | (cp & 0x3F));
    } else if (cp <= 0xFFFF) {
        result += static_cast<char>(0xE0 | (cp >> 12));
        result += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        result += static_cast<char>(0x80 | (cp & 0x3F));
    } else {
        result += static_cast<char>(0xF0 | (cp >> 18));
        result += static_cast<char>(0x80 | ((cp >> 12) & 0x3F));
        result += static_cast<char>(0x80 | ((cp >> 6) & 0x3F));
        result += static_cast<char>(0x80 | (cp & 0x3F));
    }
    return result;
}

std::string Renderer::move_cursor(int x, int y) {
    std::string move_code;
    move_code = "\e[" + std::to_string(y + 1) + ";" + std::to_string(x + 1) + "H";
    m_cursor_row_pos = x;
    m_cursor_line_pos = y;
    return move_code;
}

} // namespace dd
