#ifndef CHIP8_EMULATOR_CHIP8_HPP
#define CHIP8_EMULATOR_CHIP8_HPP

#include <cstdint>
using namespace std;

class Chip8
{
    static constexpr uint8_t REGISTER_SIZE = 16;
    static constexpr uint16_t MEMORY_SIZE = 4096;
    static constexpr uint8_t STACK_SIZE = 16;
    static constexpr uint8_t SCREEN_SIZE_X = 64;
    static constexpr uint8_t SCREEN_SIZE_Y = 32;

    uint8_t registers[REGISTER_SIZE];
    uint8_t memory[MEMORY_SIZE];
    uint16_t index_register;
    uint16_t pc;
    uint16_t stack[STACK_SIZE];
    uint8_t sp;
    uint8_t delay_timer;
    uint8_t sound_timer;
    bool screen[SCREEN_SIZE_Y][SCREEN_SIZE_X];

public:
    Chip8();

    // memory
    void loadFonts();

    // stack
    // void pushPCToStack();
    // void pullPCFromStack();

    // instructions
    void OP_00E0();
    void OP_1nnn(uint16_t mem_idx);
    void OP_6xkk(uint16_t reg_idx, uint8_t byte);
    void OP_7xkk(uint16_t reg_idx, uint8_t byte);
    void OP_Annn(uint16_t mem_idx);
    void OP_Dxyn(uint8_t x, uint8_t y, uint16_t n);

    uint16_t getMemoryAt(uint16_t address) const;
};


#endif //CHIP8_EMULATOR_CHIP8_HPP