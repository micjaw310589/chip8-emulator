#include "Chip8.hpp"

#include <cstring>
#include <bitset>

using namespace std;

void Chip8::table0()
{

}

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

    OP_table[0x0] = &Chip8::table0;
    OP_table[0x1] = &Chip8::OP_1nnn;
    OP_table[0x2] = &Chip8::OP_2nnn;
    OP_table[0x3] = &Chip8::OP_3xkk;
    OP_table[0x4] = &Chip8::OP_4xkk;
    OP_table[0x5] = &Chip8::OP_5xy0;
    OP_table[0x6] = &Chip8::OP_6xkk;
    OP_table[0x7] = &Chip8::OP_7xkk;
    // OP_table[0x8] = &Chip8::OP_8_table;
    OP_table[0xA] = &Chip8::OP_Annn;
    OP_table[0xB] = &Chip8::OP_Bnnn;
    OP_table[0xC] = &Chip8::OP_Cxkk;
    OP_table[0xD] = &Chip8::OP_Dxyn;
    // OP_table[0xE] = &Chip8::OP_E_table;
    // OP_table[0xF] = &Chip8::OP_F_table;
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

void Chip8::OP_1nnn()
// JP addr - Save Program Controller with mem_idx
{
}

void Chip8::OP_6xkk()
// LD Vx, byte - Put value kk (byte) into register Vx (reg_idx)
{
}

void Chip8::OP_7xkk()
// ADD Vx + kk - Add value kk (byte) to the value stored in Vx (reg_idx) and override it with the result
{
}

void Chip8::OP_Annn()
// LD I, addr - Set Index Register with memory idx
{
}

// void Chip8::OP_Dxyn()
// {
//     bitset<8> byte = memory[index_register];memory[index_register];
//
// }



uint16_t Chip8::getMemoryAt(const uint16_t address) const
{
    return memory[address];
}