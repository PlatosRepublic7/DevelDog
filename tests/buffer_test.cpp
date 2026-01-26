#include "buffer.h"
#include <gtest/gtest.h>

class BufferTest : public ::testing::Test {
  protected:
    dd::Buffer buf{80, 24};
};

// Verify Dimensions
TEST_F(BufferTest, DimensionsAreCorrect) {
    EXPECT_EQ(buf.get_width(), 80);
    EXPECT_EQ(buf.get_height(), 24);
}

// Test data integrity (writing and reading)
TEST_F(BufferTest, SetAndGetIntegrity) {
    dd::Style test_style;
    test_style.fg = dd::ColorName::Red;

    buf.set_cell(10, 5, {'X', test_style});

    const auto &cell = buf.get_cell(10, 5);
    EXPECT_EQ(cell.content, 'X');
    EXPECT_EQ(std::get<dd::ColorName>(cell.style.fg), dd::ColorName::Red);
}

// Test set_cell out of bounds indices
TEST_F(BufferTest, OutOfBoundsWriteDoesNotCrash) {
    EXPECT_NO_THROW(buf.set_cell(-1, 0, {'U', {}}));
    EXPECT_NO_THROW(buf.set_cell(0, -1, {'U', {}}));
    EXPECT_NO_THROW(buf.set_cell(9999, 0, {'U', {}}));
    EXPECT_NO_THROW(buf.set_cell(0, 9999, {'U', {}}));
    EXPECT_NO_THROW(buf.set_cell(-1, -1, {'U', {}}));
    EXPECT_NO_THROW(buf.set_cell(9999, 9999, {'U', {}}));
}

// Test get_cell out of bounds
TEST_F(BufferTest, OutOfBoundsReadReturnsSentinal) {
    const auto &cell = buf.get_cell(-1, 0);
    EXPECT_EQ(cell.content, ' ');

    const auto &cell2 = buf.get_cell(0, -1);
    EXPECT_EQ(cell2.content, ' ');

    const auto &cell3 = buf.get_cell(9999, 0);
    EXPECT_EQ(cell3.content, ' ');

    const auto &cell4 = buf.get_cell(0, 9999);
    EXPECT_EQ(cell4.content, ' ');

    const auto &cell5 = buf.get_cell(-1, -1);
    EXPECT_EQ(cell5.content, ' ');

    const auto &cell6 = buf.get_cell(9999, 9999);
    EXPECT_EQ(cell6.content, ' ');
}

// Test the clearing logic
TEST_F(BufferTest, ClearResetsAllCells) {
    buf.set_cell(0, 0, {'U', {}});
    buf.clear();

    EXPECT_EQ(buf.get_cell(0, 0).content, ' ');
}

// Test that clearing maintains buffer size
TEST_F(BufferTest, ClearMaintainsBufferSize) {
    size_t before_clear_size = buf.get_size();
    buf.clear();
    size_t after_clear_size = buf.get_size();

    EXPECT_EQ(before_clear_size, after_clear_size);
}

// Test resizing logic
TEST_F(BufferTest, ResizeResetsCorrectly) {
    buf.set_cell(0, 0, {'H', {}});
    buf.resize(40, 10);

    EXPECT_EQ(buf.get_width(), 40);
    EXPECT_EQ(buf.get_height(), 10);

    const auto &cell = buf.get_cell(0, 0);
    EXPECT_EQ(cell.content, ' ');
}
