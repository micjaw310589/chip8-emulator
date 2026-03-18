#include "Chip8.hpp"

#include <cstring>
#include <bitset>

using namespace std;

Chip8::Chip8()
    : registers{}
    , memory{}
    , index_register{}
    , pc{0}
    , stack{}
    , sp{0}
    , delay_timer{}
    , sound_timer{}
    , screen{}
{
    loadFonts();
}

void Chip8::loadFonts()
{
    constexpr size_t SIZE = 80;
    constexpr size_t FONT_NUM_OF_BYTES = 5;
    const uint8_t font_set[SIZE] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };
    const uint8_t* ptr_font = font_set;
    uint8_t* ptr_memory = &memory[0x050];

    while (ptr_font < font_set + SIZE)
    {
        memcpy(ptr_memory, ptr_font, sizeof(uint16_t) * FONT_NUM_OF_BYTES);
        ptr_font += FONT_NUM_OF_BYTES;
        ptr_memory += FONT_NUM_OF_BYTES;
    }
}



void Chip8::OP_00E0()
// CLS - Clear display
{
    memset(screen, 0, sizeof(screen));
}

void Chip8::OP_1nnn(const uint16_t mem_idx)
// JP addr - Save Program Controller with mem_idx
{
    if (mem_idx < MEMORY_SIZE)
    {
        pc = mem_idx;
    }
}

void Chip8::OP_6xkk(const uint16_t reg_idx, const uint8_t byte)
// LD Vx, byte - Put value kk (byte) into register Vx (reg_idx)
{
    registers[reg_idx] = byte;
}

void Chip8::OP_7xkk(const uint16_t reg_idx, const uint8_t byte)
// ADD Vx + kk - Add value kk (byte) to the value stored in Vx (reg_idx) and override it with the result
{
    registers[reg_idx] += byte;
}

void Chip8::OP_Annn(const uint16_t mem_idx)
// LD I, addr - Set Index Register with memory idx
{
    index_register = mem_idx;
}

// void Chip8::OP_Dxyn()
// {
//     bitset<8> byte = memory[index_register];
// }



uint16_t Chip8::getMemoryAt(const uint16_t address) const
{
    return memory[address];
}