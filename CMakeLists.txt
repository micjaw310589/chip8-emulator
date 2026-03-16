cmake_minimum_required(VERSION 4.0)
project(chip8_emulator)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(chip8_emulator
        src/Chip8.cpp
        src/Chip8.hpp
        tests/MemoryTestSet.cpp
        tests/MemoryTestSet.hpp
        main.cpp)
