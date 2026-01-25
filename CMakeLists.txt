cmake_minimum_required(VERSION 3.20)
project(DevelDog
    VERSION 1.0.0
    LANGUAGES CXX
    DESCRIPTION "Simple Resource Monitor for Linux"
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(SOURCES
    progress/main.cpp
    src/devel.cpp
    src/renderer.cpp
)

add_executable(devel_dog ${SOURCES})

target_include_directories(
    devel_dog PRIVATE include/dd
)

set_target_properties(devel_dog
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY
    "${CMAKE_SOURCE_DIR}/release"
)

