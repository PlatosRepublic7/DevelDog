#pragma once
#include "buffer.h"

namespace dd {
class Component {
  public:
    virtual ~Component() = default;
    // Every component must implement its own drawing logic
    virtual void draw(Buffer &buf) = 0;
};
} // namespace dd
