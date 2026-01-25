#include "color.h"
#include "devel.h"
#include "label.h"

int main() {
    dd::DevelDog dog(true);

    // Create a few labels with different styles
    dd::Style title_style;
    title_style.fg = dd::ColorName::Cyan;
    title_style.attributes = (uint8_t)dd::Attribute::Bold;

    dd::Style subtle_style;
    subtle_style.fg = dd::ColorName::White;

    // Attach the labels
    dog.attach_component(std::make_unique<dd::Label>(10, 2, "DEVELDOG TUI v0.1", title_style));
    dog.attach_component(
        std::make_unique<dd::Label>(10, 4, "Press 'q' to exit safely...", subtle_style));

    dog.start();

    return 0;
}
