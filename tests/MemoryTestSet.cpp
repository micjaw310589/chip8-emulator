#include "MemoryTestSet.hpp"
#include "../src/Chip8.hpp"
#include <iostream>
#include <bitset>

bool MemoryTestSet::test_LoadFont()
// tries to read 3 characters from memory (first, middle, last) and checks for pattern match.
{
    // expected output
    const uint16_t expected_0[5] = {0xF0, 0x90, 0x90, 0x90, 0xF0};
    const uint16_t expected_A[5] = {0xF0, 0x90, 0xF0, 0x90, 0x90};
    const uint16_t expected_F[5] = {0xF0, 0x80, 0xF0, 0x80, 0x80};

    std::cerr << "test_LoadFont:" << std::endl;

    const Chip8 chip8;    // font should be loaded into memory after executing constructor code

    bool fError = false;


    std::cerr << "reading font..." << std::endl;
    std::cerr << "\n0:" << std::endl;
    // read 0
    for (uint16_t i = 0; i < 5; i++)
    {
        const uint16_t char_byte = chip8.getMemoryAt(0x050 + i);
        if (char_byte != expected_0[i])
        {
            fError = true;
        }
        const std::bitset<8> char_byte_bitset = char_byte;
        std::cerr << char_byte_bitset << std::endl;
    }

    if (fError)
    {
        std::cerr << "read value is not right" << std::endl;
        std::cerr << "test_LoadFont: FAIL!" << std::endl;
        return false;
    }


    std::cerr << "\nA:" << std::endl;
    // read A
    for (uint16_t i = 0; i < 5; i++)
    {
        const uint16_t char_byte = chip8.getMemoryAt(0x050 + 5*0xA + i);
        if (char_byte != expected_A[i])
        {
            fError = true;
        }
        const std::bitset<8> char_byte_bitset = char_byte;
        std::cerr << char_byte_bitset << std::endl;

    }

    if (fError)
    {
        std::cerr << "read value is not right" << std::endl;
        std::cerr << "test_LoadFont: FAIL!" << std::endl;
        return false;
    }


    std::cerr << "\nF:" << std::endl;
    // read F
    for (uint16_t i = 0; i < 5; i++)
    {
        const uint16_t char_byte = chip8.getMemoryAt(0x050 + 5*0xF + i);
        if (char_byte != expected_F[i])
        {
            fError = true;
        }
        const std::bitset<8> char_byte_bitset = char_byte;
        std::cerr << char_byte_bitset << std::endl;
    }

    if (fError)
    {
        std::cerr << "read value is not right" << std::endl;
        std::cerr << "test_LoadFont: FAIL!" << std::endl;
        return false;
    }


    std::cerr << "test_LoadFont: ok" << std::endl;
    return true;
}