#include "Chip8.hpp"

#include <cstring>
#include <bitset>
#include <iostream>

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
    , opcode{0x2121}
{
    loadFonts();

    OP_table[0x0] = &Chip8::table0;
    OP_table[0x1] = &Chip8::OP_1nnn;
    OP_table[0x2] = &Chip8::OP_2nnn;
    OP_table[0x3] = &Chip8::OP_3xkk;
    // OP_table[0x4] = &Chip8::OP_4xkk;
    // OP_table[0x5] = &Chip8::OP_5xy0;
    // OP_table[0x6] = &Chip8::OP_6xkk;
    // OP_table[0x7] = &Chip8::OP_7xkk;
    // // OP_table[0x8] = &Chip8::OP_8_table;
    // OP_table[0xA] = &Chip8::OP_Annn;
    // OP_table[0xB] = &Chip8::OP_Bnnn;
    // OP_table[0xC] = &Chip8::OP_Cxkk;
    // OP_table[0xD] = &Chip8::OP_Dxyn;
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

void Chip8::OP_00EE()
// RET - Return from a subroutine
{
    pc = sp--;
}

void Chip8::OP_1nnn()
// JP addr - Jump to location nnn
{
    const uint16_t addr = opcode & 0x0FFFu;
    pc = addr;
}

void Chip8::OP_2nnn()
// CALL addr - Call subroutine at nnn
{
    const uint16_t addr = opcode & 0x0FFFu;
    stack[sp++] = pc;
    pc = addr;
}

void Chip8::OP_3xkk()
// SE Vx, byte - Skip next instruction if Vx == kk
{
    const auto x = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
    const auto byte = static_cast<uint8_t>(opcode & 0x00FFu);

    if (registers[x] == byte)
    {
        pc += 2;
    }
}

void Chip8::OP_4xkk()
// SNE Vx, byte - Skip next instruction if Vx != kk
{
    const auto x = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
    const auto byte = static_cast<uint8_t>(opcode & 0x00FFu);

    if (registers[x] != byte)
    {
        pc += 2;
    }
}

void Chip8::OP_5xy0()
// SE Vx, Vy - Skip next instruction if Vx == Vy
{
    const auto x = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
    const auto y = static_cast<uint8_t>((opcode & 0x00F0u) >> 4u);

    if (registers[x] == registers[y])
    {
        pc += 2;
    }
}

void Chip8::OP_6xkk()
// LD Vx, byte - Set Vx = kk
{
    const auto x = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
    const auto byte = static_cast<uint8_t>(opcode & 0x00FFu);

    registers[x] = byte;
}

void Chip8::OP_7xkk()
// ADD Vx, byte - Set Vx = Vx + kk
{
    const auto x = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
    const auto byte = static_cast<uint8_t>(opcode & 0x00FFu);

    registers[x] += byte;
}

void Chip8::OP_8xy0()
// LD Vx, Vy - Set Vx = Vy
{
    const auto x = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
    const auto y = static_cast<uint8_t>((opcode & 0x00F0u) >> 4u);

    registers[x] = registers[y];
}

void Chip8::OP_8xy1()
// OR Vx, Vy - Set Vx = Vx OR Vy
{
    const auto x = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
    const auto y = static_cast<uint8_t>((opcode & 0x00F0u) >> 4u);

    registers[x] |= registers[y];
}

void Chip8::OP_8xy2()
// AND Vx, Vy - Set Vx = Vx AND Vy
{
    const auto x = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
    const auto y = static_cast<uint8_t>((opcode & 0x00F0u) >> 4u);

    registers[x] &= registers[y];
}

void Chip8::OP_8xy3()
// XOR Vx, Vy - Set Vx = Vx XOR Vy
{
    const auto x = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
    const auto y = static_cast<uint8_t>((opcode & 0x00F0u) >> 4u);

    registers[x] ^= registers[y];
}

void Chip8::OP_8xy4()
// ADD Vx, Vy - Set Vx = Vx + Vy, set VF = carry.
{
    const auto x = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
    const auto y = static_cast<uint8_t>((opcode & 0x00F0u) >> 4u);

    const uint16_t result = registers[x] + registers[y];

    if (result > 255u) {
        registers[0xF] = 0x01u;
    } else {
        registers[0xF] = 0x00u;
    }

    registers[x] = 0xFFu & result;
}

void Chip8::OP_8xy5()
// SUB Vx, Vy - Set Vx = Vx - Vy, set VF = NOT borrow.
{
    const auto x = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
    const auto y = static_cast<uint8_t>((opcode & 0x00F0u) >> 4u);

    if (registers[x] > registers[y]) {
        registers[0xF] = 0x01u;
    } else {
        registers[0xF] = 0x00u;
    }

    registers[x] -= registers[y];
}

void Chip8::OP_8xy6()
// SHR Vx {, Vy} - Set Vx = Vx SHR 1
{
    const auto x = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);

    registers[0xF] = registers[x] & 0x01u;

    registers[x] >>= 1u;
}

void Chip8::OP_8xy7()
// SUBN Vx, Vy - Set Vx = Vy - Vx, set VF = NOT borrow
{
    const auto x = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
    const auto y = static_cast<uint8_t>((opcode & 0x00F0u) >> 4u);

    if (registers[x] < registers[y]) {
        registers[0xF] = 0x01u;
    } else {
        registers[0xF] = 0x00u;
    }

    registers[x] = registers[y] - registers[x];
}

void Chip8::OP_8xyE()
// SHL Vx {, Vy} - Set Vx = Vx SHL 1
{
    const auto x = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);

    registers[0xF] = registers[x] & 0x80u;

    registers[x] <<= 1u;
}

void Chip8::OP_9xy0()
// SNE Vx, Vy - Skip next instruction if Vx != Vy
{
    const auto x = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
    const auto y = static_cast<uint8_t>((opcode & 0x00F0u) >> 4u);

    if (registers[x] != registers[y]) {
        pc += 2;
    }
}

void Chip8::OP_Annn()
// LD I, addr - Set I = nnn.
{
    const uint16_t addr = opcode & 0x0FFFu;

    index_register = addr;
}

void Chip8::OP_Bnnn()
// JP V0, addr - Jump to location nnn + V0
{
    const uint16_t addr = opcode & 0x0FFFu;

    pc = addr + registers[0x0];
}

void Chip8::OP_Cxkk()
// RND Vx, byte - Set Vx = random byte AND kk
{
    // rng
}

void Chip8::OP_Dxyn()
// DRW Vx, Vy, nibble - Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
{
    const auto x = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
    const auto y = static_cast<uint8_t>((opcode & 0x00F0u) >> 4u);
    const uint8_t num_of_bytes = opcode & 0x000Fu;

    for (uint8_t i = 0; i < num_of_bytes; i++) {
        const uint8_t byte = memory[index_register + i];
        const uint8_t x_px = registers[x];
        const uint8_t y_px = registers[y];

        uint8_t mask = 0x01u;
        for (int j = 0; j < 8; j++) {
            const uint32_t pixel = screen[y_px][x_px];
            const uint8_t bit = byte & mask;
            registers[0xF] = (pixel == 0x01u == bit) ? 0x01u : 0x00u;

            screen[y_px][x_px] ^= bit;

            mask <<= 1u;
        }


    }
}



uint16_t Chip8::getMemoryAt(const uint16_t address) const
{
    return memory[address];
}