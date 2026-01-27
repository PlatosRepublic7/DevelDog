#pragma once

namespace dd {
namespace Border {
// Single Line (The standard look)
constexpr char32_t Single_TL = U'┌';
constexpr char32_t Single_TR = U'┐';
constexpr char32_t Single_BL = U'└';
constexpr char32_t Single_BR = U'┘';
constexpr char32_t Single_H = U'─';
constexpr char32_t Single_V = U'│';

// Double Line (For high-contrast or headers)
constexpr char32_t Double_TL = U'╔';
constexpr char32_t Double_TR = U'╗';
constexpr char32_t Double_BL = U'╚';
constexpr char32_t Double_BR = U'╝';
constexpr char32_t Double_H = U'═';
constexpr char32_t Double_V = U'║';

// Rounded Corners (Modern/Sleek look)
constexpr char32_t Round_TL = U'╭';
constexpr char32_t Round_TR = U'╮';
constexpr char32_t Round_BL = U'╰';
constexpr char32_t Round_BR = U'╯';
} // namespace Border
} // namespace dd
