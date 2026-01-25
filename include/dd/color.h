#pragma once
#include <cstdint>
#include <variant>

namespace dd {

enum class ColorName : uint8_t {
    Default = 0,
    Black,
    Red,
    Green,
    Yellow,
    Blue,
    Magenta,
    Cyan,
    White,
    BrightBlack,
    BrightRed,
    BrightGreen,
    BrightYellow,
    BrightBlue,
    BrightMagenta,
    BrightCyan,
    BrightWhite
};

struct RGB {
    uint8_t r, g, b;

    bool operator==(const RGB &) const = default;
};

struct Palette {
    uint8_t index;

    bool operator==(const Palette &) const = default;
};

// Master Color Type
using Color = std::variant<ColorName, Palette, RGB>;

enum class Attribute : uint8_t {
    None = 0,
    Bold = 1 << 0,
    Dim = 1 << 1,
    Underline = 1 << 2,
    Blink = 1 << 3,
    Reverse = 1 << 4,
    Hidden = 1 << 5
};

struct Style {
    Color fg = ColorName::Default;
    Color bg = ColorName::Default;
    uint8_t attributes = 0; // bitmask of Attribute

    // Helper to check for attributes
    bool has_attr(Attribute attr) const { return attributes & static_cast<uint8_t>(attr); }

    bool operator==(const Style &) const = default;
};

} // namespace dd
