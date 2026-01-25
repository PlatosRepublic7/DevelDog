#include "box.h"
#include "devel.h"
#include "label.h"

int main() {
    dd::DevelDog dog;

    dd::Style box_style;
    box_style.fg = dd::ColorName::Green;

    dd::Style text_style;
    text_style.fg = dd::ColorName::White;
    text_style.attributes = (uint8_t)dd::Attribute::Bold;

    // Attach a large box
    dog.attach_component(std::make_unique<dd::Box>(5, 2, 40, 10, box_style));

    // Attach a label within the box
    dog.attach_component(std::make_unique<dd::Label>(8, 4, "SYSTEM STATUS: ACTIVE", text_style));

    dog.start();

    return 0;
}
