#include "renderer.h"
#include <variant>

namespace dd {
std::string Renderer::render(const Buffer &buffer, bool debug_state) {
    std::string output;
    char null_char = ' ';

    if (debug_state) {
        null_char = '.';
    }

    output.reserve(buffer.get_width() * buffer.get_height() * 12);

    // Move cursor to 1, 1 without clearing
    output += "\e[H";

    for (int y = 0; y < buffer.get_height(); ++y) {
        for (int x = 0; x < buffer.get_width(); ++x) {
            const Cell &cell = buffer.get_cell(x, y);

            output += format_style(cell.style);

            // If content is null or 0, draw a space
            if (cell.content == 0 || cell.content == ' ') {
                output += null_char;
            } else {
                output += static_cast<char>(cell.content);
            }
        }

        if (y < buffer.get_height() - 1) {
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

    // Attribute logic
    if (style.attributes & static_cast<uint8_t>(Attribute::Bold)) {
        ansi += "\e[1m";
    }

    return ansi;
}

} // namespace dd
