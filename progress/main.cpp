#include "box.h"
#include "devel.h"
#include "label.h"

int main() {
    dd::DevelDog devel_term(true);

    dd::Style box_style;
    box_style.fg = dd::RGB{221, 134, 58}; // Halloween Orange

    dd::Style text_style;
    text_style.fg = dd::ColorName::White;
    text_style.attributes = (uint8_t)dd::Attribute::Bold;

    // Attach a large box
    devel_term.attach_component(
        std::make_unique<dd::Box>(0, 0, 40, 10, box_style));

    // Attach a label within the box
    devel_term.attach_component(
        std::make_unique<dd::Label>(8, 4, "SYSTEM STATUS: ACTIVE", text_style));

    devel_term.start();

    return 0;
}
