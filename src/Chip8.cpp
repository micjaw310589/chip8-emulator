#include "Chip8.hpp"
#include <cstring>
#include <chrono>
// #include <iostream>

using namespace std;



void Chip8::table0()
{
    (this->*OP_0_table[opcode & 0x000Fu])();
}

void Chip8::table8()
{
    (this->*OP_8_table[opcode & 0x000Fu])();
}

void Chip8::tableE()
{
    (this->*OP_E_table[opcode & 0x000Fu])();
}

void Chip8::tableF()
{
    (this->*OP_F_table[opcode & 0x00FFu])();
}

uint8_t Chip8::generateRandomNumber()
{
    return dist_byte(rng);
}

void Chip8::loadFonts()
{
    constexpr size_t SIZE = 80;
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

    memcpy(&memory[FONT_BEGINNING_IDX], font_set, sizeof(font_set[0]) * SIZE);
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
    , keypad{}
    , OP_table{&Chip8::OP_NULL}
    , OP_0_table{&Chip8::OP_NULL}
    , OP_8_table{&Chip8::OP_NULL}
    , OP_E_table{&Chip8::OP_NULL}
    , OP_F_table{&Chip8::OP_NULL}
    , opcode{PROG_MEM_BEGINNING_IDX}
    , rng{static_cast<mt19937::result_type>(
        chrono::steady_clock::now()
        .time_since_epoch()
        .count()
    )}
    , time{std::chrono::system_clock::now()}
{
    OP_table[0x0] = &Chip8::table0;
    OP_table[0x1] = &Chip8::OP_1nnn;
    OP_table[0x2] = &Chip8::OP_2nnn;
    OP_table[0x3] = &Chip8::OP_3xkk;
    OP_table[0x4] = &Chip8::OP_4xkk;
    OP_table[0x5] = &Chip8::OP_5xy0;
    OP_table[0x6] = &Chip8::OP_6xkk;
    OP_table[0x7] = &Chip8::OP_7xkk;
    OP_table[0x8] = &Chip8::table8;
    OP_table[0xA] = &Chip8::OP_Annn;
    OP_table[0xB] = &Chip8::OP_Bnnn;
    OP_table[0xC] = &Chip8::OP_Cxkk;
    OP_table[0xD] = &Chip8::OP_Dxyn;
    OP_table[0xE] = &Chip8::tableE;
    OP_table[0xF] = &Chip8::tableF;

    OP_0_table[0x0] = &Chip8::OP_00E0;
    OP_0_table[0xE] = &Chip8::OP_00EE;

    OP_8_table[0x0] = &Chip8::OP_8xy0;
    OP_8_table[0x1] = &Chip8::OP_8xy1;
    OP_8_table[0x2] = &Chip8::OP_8xy2;
    OP_8_table[0x3] = &Chip8::OP_8xy3;
    OP_8_table[0x4] = &Chip8::OP_8xy4;
    OP_8_table[0x5] = &Chip8::OP_8xy5;
    OP_8_table[0x6] = &Chip8::OP_8xy6;
    OP_8_table[0x7] = &Chip8::OP_8xy7;
    OP_8_table[0xE] = &Chip8::OP_8xyE;

    OP_E_table[0x1] = &Chip8::OP_ExA1;
    OP_E_table[0xE] = &Chip8::OP_Ex9E;

    OP_F_table[0x07] = &Chip8::OP_Fx07;
    OP_F_table[0x0A] = &Chip8::OP_Fx0A;
    OP_F_table[0x15] = &Chip8::OP_Fx15;
    OP_F_table[0x18] = &Chip8::OP_Fx18;
    OP_F_table[0x1E] = &Chip8::OP_Fx1E;
    OP_F_table[0x29] = &Chip8::OP_Fx29;
    OP_F_table[0x33] = &Chip8::OP_Fx33;
    OP_F_table[0x55] = &Chip8::OP_Fx55;
    OP_F_table[0x65] = &Chip8::OP_Fx65;

    loadFonts();
}



void Chip8::cycle()
{
    opcode = memory[pc] << 8u | memory[pc + 1];
    pc += 2;

    const uint8_t idx = (opcode & 0xF000u) >> 12u;
    (this->*OP_table[idx])();

    if (delay_timer > 0) {
        delay_timer--;
    }

    if (sound_timer > 0) {
        sound_timer--;
    }
}



void Chip8::OP_NULL()
// NULL - does nothing (filler)
{
    return;
}

void Chip8::OP_00E0()
// CLS - Clear display
{
    memset(screen, 0, sizeof(screen));
}

void Chip8::OP_00EE()
// RET - Return from a subroutine
{
    pc = stack[sp--];
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
    const auto x = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
    const auto byte = static_cast<uint8_t>(opcode & 0x00FFu);

    registers[x] = generateRandomNumber() & byte;
}

void Chip8::OP_Dxyn()
// DRW Vx, Vy, nibble - Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision
{
    const auto x = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);
    const auto y = static_cast<uint8_t>((opcode & 0x00F0u) >> 4u);
    const uint8_t num_of_bytes = opcode & 0x000Fu;

    registers[0xF] = 0x00u;

    for (uint8_t i = 0; i < num_of_bytes; i++) {
        const uint8_t sprite_byte = memory[index_register + i];
        const uint8_t x_px = registers[x] & (SCREEN_SIZE_X - 1);
        const uint8_t y_px = registers[y] & (SCREEN_SIZE_Y - 1) + i;

        const uint16_t screen_idx = y_px * SCREEN_SIZE_X + x_px;

        uint8_t mask = 0b10000000u;

        for (uint8_t b = sizeof(sprite_byte); b > 0; b--) {
            const uint8_t px_bit = (sprite_byte & mask) >> (b - 1);
            const uint32_t sprite_px = (px_bit == 0x00u) ? 0x00000000u : 0xFFFFFFFFu;
            const uint32_t display_px = screen[screen_idx] ^ sprite_px;

            if ((sprite_px & display_px) == 0x00000000u) {
                registers[0xF] = 0x01u;
            }

            screen[screen_idx] = display_px;
            mask >>= 1u;
        }
    }
}

void Chip8::OP_Ex9E()
// SKP Vx - Skip next instruction if key with the value of Vx is pressed
{
    const auto x = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);

    const uint8_t key = registers[x];

    if (keypad[key]) {
        pc += 2;
    }
}

void Chip8::OP_ExA1()
// SKNP Vx - Skip next instruction if key with the value of Vx is not pressed
{
    const auto x = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);

    const uint8_t key = registers[x];

    if (!keypad[key]) {
        pc += 2;
    }
}

void Chip8::OP_Fx07()
// LD Vx, DT - Set Vx = delay timer value
{
    const auto x = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);

    registers[x] = delay_timer;
}

void Chip8::OP_Fx0A()
// LD Vx, K - Wait for a key press, store the value of the key in Vx
{
    const auto x = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);

    for (uint8_t i = 0; i < KEYPAD_SIZE; i++) {
        if (keypad[i]) {
            registers[x] = i;
            break;
        }

        pc -= 2;
    }
}

void Chip8::OP_Fx15()
// LD DT, Vx - Set delay timer = Vx
{
    const auto x = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);

    delay_timer = registers[x];
}

void Chip8::OP_Fx18()
// LD ST, Vx - Set sound timer = Vx
{
    const auto x = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);

    sound_timer = registers[x];
}

void Chip8::OP_Fx1E()
// ADD I, Vx - Set I = I + Vx
{
    const auto x = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);

    index_register += registers[x];
}

void Chip8::OP_Fx29()
// LD F, Vx - Set I = location of sprite for digit Vx
{
    const auto x = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);

    const uint8_t sprite = registers[x];    // [0x00u ; 0x0Fu]

    index_register = FONT_BEGINNING_IDX + sprite;
}

void Chip8::OP_Fx33()
// LD B, Vx - Store BCD representation of Vx in memory locations I, I+1, and I+2
{
    const auto x = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);

    uint8_t val = registers[x];

    for (int i = 3; i > 0; i--) {
        memory[index_register + (i-1)] = val % 10u;
        val /= 10u;
    }
}

void Chip8::OP_Fx55()
// LD [I], Vx - Store registers V0 through Vx in memory starting at location I
{
    const auto x = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);

    memcpy(memory + index_register, registers, sizeof(registers[0]) * x);
}

void Chip8::OP_Fx65()
// LD Vx, [I] - Read registers V0 through Vx from memory starting at location I
{
    const auto x = static_cast<uint8_t>((opcode & 0x0F00u) >> 8u);

    memcpy(registers, memory + index_register, sizeof(registers[0]) * x);
}


uint16_t Chip8::getMemoryAt(const uint16_t address) const
{
    return memory[address];
}