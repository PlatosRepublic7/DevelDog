#include <gtest/gtest.h>

// A simple test that always passes
TEST(BasicSetup, MathWorks) { EXPECT_EQ(2 + 2, 4); }

// A simple test to prove GTest is running
TEST(BasicSetup, StringCheck) {
    std::string name = "DevelDog";
    ASSERT_EQ(name, "DevelDog");
}
