#include "renderer.h"
#include <variant>

namespace dd {
std::string Renderer::render(const Buffer &back, const Buffer &front, bool debug_state) {
    std::string output;
    char null_char = ' ';

    if (debug_state) {
        null_char = '.';
    }

    int width = back.get_width();
    int height = back.get_height();
    output.reserve(width * height * m_RESERVE_CONST);

    // Move cursor to 1, 1 without clearing
    output += "\e[H";

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const Cell &new_cell = back.get_cell(x, y);
            const Cell &old_cell = front.get_cell(x, y);
            Cell cell_to_draw = {};

            // This is the beginning of the diffing logic
            // it is by no means perfect, we are instead looking for a direct
            // working replacement of what we had, albeit with a second buffer
            if (new_cell != old_cell) {
                cell_to_draw = new_cell;
            } else {
                cell_to_draw = old_cell;
            }

            output += format_style(cell_to_draw.style);

            // If content is null or 0, draw a space
            if (cell_to_draw.content == 0 || cell_to_draw.content == ' ') {
                output += null_char;
            } else {
                output += static_cast<char>(cell_to_draw.content);
            }
        }

        if (y < back.get_height() - 1) {
            output += "\r\n";
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

} // namespace dd
