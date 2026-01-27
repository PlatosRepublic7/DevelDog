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

# Define some options so we can toggle features for the build configuration
option(BUILD_SHARED_LIBS "Build Shared Libraries" OFF)
option(BUILD_APP "Build progress/main.cpp app" ON)
option(BUILD_TESTS "Build Unit Tests" ON)

# The Library
set(LIB_SOURCES
    src/devel.cpp
    src/renderer.cpp
    src/buffer.cpp
)

# Always build the static library
add_library(dd_static STATIC ${LIB_SOURCES})

set_target_properties(dd_static PROPERTIES
    OUTPUT_NAME "dd"
    DEBUG_POSTFIX "_debug"  # This will only apply to Debug builds
)

target_include_directories(dd_static PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/dd>
    $<INSTALL_INTERFACE:include/dd>
)

set(install_targets dd_static)
set(lib_to_link dd_static)

# Optionally build the shared library
if(BUILD_SHARED_LIBS)
    add_library(dd_shared SHARED ${LIB_SOURCES})
    set_target_properties(dd_shared PROPERTIES
        OUTPUT_NAME "dd"
        DEBUG_POSTFIX "_debug"
    )

    target_include_directories(dd_shared PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/dd>
        $<INSTALL_INTERFACE:include/dd>
    )
    list(APPEND install_targets dd_shared)
endif()

# Build the app
if(BUILD_APP)
    add_executable(devel_dog progress/main.cpp)
    target_link_libraries(devel_dog PRIVATE ${lib_to_link})
    list(APPEND install_targets devel_dog)
endif()

# Installation
install(TARGETS ${install_targets}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# This handles the headers
install(DIRECTORY include/dd DESTINATION include)

# Tests
if(BUILD_TESTS)
    include(FetchContent)
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
        GIT_SHALLOW TRUE
    )

    FetchContent_MakeAvailable(googletest)
    # For Windows compatibility, it's good practice
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    enable_testing()

    add_subdirectory(tests)

    #    add_test(NAME CoreLogicTest COMMAND unit_tests)
endif()

