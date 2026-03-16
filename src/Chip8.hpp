#ifndef CHIP8_EMULATOR_CHIP8_HPP
#define CHIP8_EMULATOR_CHIP8_HPP

#include <cstdint>

class Chip8
{
    uint8_t registry[16];
    uint16_t memory[4096];
    uint16_t index_register;
    uint16_t* pc = nullptr;
    uint16_t* stack[16];
    uint16_t** sp = nullptr;
    uint8_t delay_timer;
    uint8_t sound_timer;
    bool screen[64 * 32];

public:
    Chip8();

    // memory
    void loadFonts();

    // stack
    // void pushPCToStack();
    // void pullPCFromStack();

    // instructions
    void OP_00E0();
    void OP_1nnn(uint16_t* addr);
    void OP_6xkk();
    void OP_7xkk();
    void OP_Annn();
    void OP_DXYN();

    uint16_t getMemoryAt(uint16_t address) const;
};


#endif //CHIP8_EMULATOR_CHIP8_HPP