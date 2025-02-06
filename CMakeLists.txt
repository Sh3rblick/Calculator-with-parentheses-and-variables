cmake_minimum_required(VERSION 3.30)
project(untitled)

set(CMAKE_CXX_STANDARD 20)

# Add Google Test
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(untitled main.cpp main.h
        test.cpp
)

# Link Google Test libraries
target_link_libraries(untitled gtest gtest_main)

# Add test
add_executable(untitled_test test.cpp)
target_link_libraries(untitled_test gtest gtest_main)
add_test(NAME untitled_test COMMAND untitled_test)