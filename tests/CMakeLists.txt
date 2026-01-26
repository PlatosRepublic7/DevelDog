# Add test files to this list as we move forward
add_executable(unit_tests
    sanity_check.cpp
    renderer_test.cpp
    buffer_test.cpp
)

target_link_libraries(unit_tests PRIVATE dd_lib GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_tests)

