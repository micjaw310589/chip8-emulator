#ifndef CHIP8_EMULATOR_CHIP8_HPP
#define CHIP8_EMULATOR_CHIP8_HPP

#include <cstdint>
using namespace std;

class Chip8
{
    using op_ptr = void (Chip8::*)();

    static constexpr uint8_t REGISTER_SIZE = 16;
    static constexpr uint16_t MEMORY_SIZE = 4096;
    static constexpr uint8_t STACK_SIZE = 16;
    static constexpr uint8_t SCREEN_SIZE_X = 64;
    static constexpr uint8_t SCREEN_SIZE_Y = 32;

    static constexpr uint8_t OP_SIZE = 16;
    static constexpr uint8_t OP_0_SIZE = 2;
    static constexpr uint8_t OP_8_SIZE = 9;
    static constexpr uint8_t OP_E_SIZE = 2;
    static constexpr uint8_t OP_F_SIZE = 9;

    uint8_t registers[REGISTER_SIZE];
    uint8_t memory[MEMORY_SIZE];
    uint16_t index_register;
    uint16_t pc;
    uint16_t stack[STACK_SIZE];
    uint8_t sp;
    uint8_t delay_timer;
    uint8_t sound_timer;
    bool screen[SCREEN_SIZE_Y][SCREEN_SIZE_X];

    // instruction tables
    uint16_t opcode;
    op_ptr OP_table[OP_SIZE];
    op_ptr OP_0_table[OP_0_SIZE];
    op_ptr OP_8_table[OP_8_SIZE];
    op_ptr OP_E_table[OP_E_SIZE];
    op_ptr OP_F_table[OP_F_SIZE];

    void table0();
    // void table8();
    // void tableE();
    // void tableF();

public:
    Chip8();

    // memory
    void loadFonts();

    // stack
    // void pushPCToStack();
    // void pullPCFromStack();

    // instructions
    void OP_00E0();     // CLS
    void OP_00EE();     // RET
    void OP_1nnn();     // JP addr
    void OP_2nnn();     // CALL addr
    void OP_3xkk();     // SE Vx, byte
    void OP_4xkk();     // SNE Vx, byte
    void OP_5xy0();     // SE Vx, Vy
    void OP_6xkk();     // LD Vx, byte
    void OP_7xkk();     // ADD Vx, byte
    void OP_8xy0();     // LD Vx, Vy
    void OP_8xy1();     // OR Vx, Vy
    void OP_8xy2();     // AND Vx, Vy
    void OP_8xy3();     // XOR Vx, Vy
    void OP_8xy4();     // ADD Vx, Vy
    void OP_8xy5();     // SUB Vx, Vy
    void OP_8xy6();     // SHR Vx {, Vy}
    void OP_8xy7();     // SUBN Vx, Vy
    void OP_8xyE();     // SHL Vx {, Vy}
    void OP_9xy0();     // SNE Vx, Vy
    void OP_Annn();     // LD I, addr
    void OP_Bnnn();     // JP V0, addr
    void OP_Cxkk();     // RND Vx, byte
    void OP_Dxyn();     // DRW Vx, Vy, nibble
    void OP_Ex9E();     // SKP Vx
    void OP_ExA1();     // SKNP Vx
    void OP_Fx07();     // LD Vx, DT
    void OP_Fx0A();     // LD Vx, K
    void OP_Fx15();     // LD DT, Vx
    void OP_Fx18();     // LD ST, Vx
    void OP_Fx1E();     // ADD I, Vx
    void OP_Fx29();     // LD F, Vx
    void OP_Fx33();     // LD [I], Vx
    void OP_Fx55();     // LD [I], Vx
    void OP_Fx65();     // LD Vx, [I]

    // debug
    uint16_t getMemoryAt(uint16_t address) const;
};


#endif //CHIP8_EMULATOR_CHIP8_HPP