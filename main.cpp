// #include "tests/MemoryTestSet.hpp"
#include "src/Chip8.hpp"
#include "src/Display.hpp"
#include <SDL3/SDL.h>

int main(int argc, char* argv[])
{
    constexpr uint16_t PHYSICAL_WIDTH_PX = 1200;
    constexpr uint16_t PHYSICAL_HEIGHT_PX = 600;
    constexpr uint16_t LOGICAL_WIDTH_PX = 64;
    constexpr uint16_t LOGICAL_HEIGHT_PX = 32;

    Display display("Chip-8 Emulator", PHYSICAL_WIDTH_PX, PHYSICAL_HEIGHT_PX,
                    LOGICAL_WIDTH_PX, LOGICAL_HEIGHT_PX);

    if (!display.isInitialized()) {
        return SDL_APP_FAILURE;
    }

    Chip8 chip8;
    if (!chip8.loadROM("../test-roms/pong-1-player.ch8")) {
        SDL_Log("Failed to load ROM!");
        return SDL_APP_FAILURE;
    }

    while (display.processInput(chip8)) {
        SDL_Delay(16); // about 60Hz

        chip8.cycle();

        display.render(chip8.getScreenAddr());
    }

    return 0;
}