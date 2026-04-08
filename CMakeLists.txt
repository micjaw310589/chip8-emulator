cmake_minimum_required(VERSION 4.0)
project(chip8_emulator)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)

add_executable(chip8_emulator
        src/Chip8.cpp
        src/Chip8.hpp
        tests/MemoryTestSet.cpp
        tests/MemoryTestSet.hpp
        main.cpp
        src/Display.cpp
        src/Display.hpp
)

find_package(SDL3 REQUIRED)
target_link_libraries(chip8_emulator PRIVATE SDL3::SDL3)