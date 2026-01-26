#include "buffer.h"
#include "color.h"
#include "renderer.h"
#include <gtest/gtest.h>

class RendererTest : public ::testing::Test {
  protected:
    dd::Renderer renderer;
};

// Test basic character rendering
TEST_F(RendererTest, RendersPlainCharacter) {
    dd::Buffer buf(1, 1);
    buf.set_cell(0, 0, {'A', dd::Style{}});

    std::string output = renderer.render(buf, false);

    // Should contain the character 'A'
    EXPECT_NE(output.find('A'), std::string::npos);
}

// Test ColorName
TEST_F(RendererTest, RendersRGBForeground) {
    dd::Buffer buf(1, 1);
    dd::Style style;
    style.fg = dd::RGB{255, 128, 64};
    buf.set_cell(0, 0, {'#', style});

    std::string output = renderer.render(buf, false);

    // Sequence format: \e[38;2;R;G;Bm
    EXPECT_NE(output.find("38;2;255;128;64m"), std::string::npos);
}

// Test RGB Background
TEST_F(RendererTest, RendersRGBBackground) {
    dd::Buffer buf(1, 1);
    dd::Style style;
    style.bg = dd::RGB{255, 128, 64};
    buf.set_cell(0, 0, {'#', style});

    std::string output = renderer.render(buf, false);

    // Sequence format: \e[48;2;R;G;Bm
    EXPECT_NE(output.find("48;2;255;128;64m"), std::string::npos);
}

// Test Attributes
TEST_F(RendererTest, RendersBoldAttribute) {
    dd::Buffer buf(1, 1);
    dd::Style style;
    style.attributes = static_cast<uint8_t>(dd::Attribute::Bold);
    buf.set_cell(0, 0, {'?', style});

    std::string output = renderer.render(buf, false);

    // Sequence for bold is \e[1m
    EXPECT_NE(output.find("1m"), std::string::npos);
}

// Test that two cells with different colors next to one another dont
// "bleed" color. The renderer should insert a reset code \e[0m or \e[39m
// before the second character
TEST_F(RendererTest, RendersResetCharacter) {
    dd::Buffer buf(2, 1);
    dd::Style style1;
    dd::Style style2;
    style1.fg = dd::ColorName::Red;
    style1.fg = dd::ColorName::Blue;
    buf.set_cell(0, 0, {'R', style1});
    buf.set_cell(1, 0, {'B', style2});

    std::string output = renderer.render(buf, false);

    bool found_code = false;
    if (output.find("0m") != std::string::npos || output.find("39m") != std::string::npos) {
        found_code = true;
    }

    EXPECT_EQ(found_code, true);
}
