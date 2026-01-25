#pragma once
#include "buffer.h"
#include <string>

namespace dd {
class Renderer {
  public:
    Renderer() = default;

    // Compile the entire buffer into one ANSI-encoded string
    std::string render(const Buffer &buffer);

  private:
    // Helper to turn Style struct into ANSI escape codes
    std::string format_style(const Style &style);
};
} // namespace dd
