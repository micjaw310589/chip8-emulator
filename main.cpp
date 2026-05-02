// #include "tests/MemoryTestSet.hpp"

#include "src/Chip8.hpp"
#include "src/Display.hpp"
#include <SDL3/SDL.h>
#include <sstream>
#include <iostream>

// ./chip8-emulator [ROM_filepath] [cpu_hz] [height_px]
int main(int argc, char* argv[])
{
    constexpr uint16_t LOGICAL_WIDTH_PX = 64;
    constexpr uint16_t LOGICAL_HEIGHT_PX = 32;
    uint16_t PHYSICAL_HEIGHT_PX = 1080;
    uint16_t PHYSICAL_WIDTH_PX = 1920;

    if (argc > 4) {
        std::cerr << "Too many arguments.\n";
        return 1;
    }

    const char* filepath = argv[1];
    uint16_t h_res_px = 0;
    bool isFullscreen = false;
    stringstream convert_hz(argv[2]);
    int clock_hz = 0;
    if (!(convert_hz >> clock_hz)) {clock_hz = 60;}

    if (clock_hz < 60 || 1000 < clock_hz) {
        std::cerr << "Clock speed should be between 60 and 1000 Hz.\n";
        return 1;
    }

    if (argc == 4) {
        stringstream convert_px(argv[3]);
        if (!(convert_px >> h_res_px)) { isFullscreen = true; }
    }
    else {
        isFullscreen = true;
    }

    if (!isFullscreen) {
        PHYSICAL_HEIGHT_PX = h_res_px;
        PHYSICAL_WIDTH_PX = PHYSICAL_HEIGHT_PX / 9 * 16;
    }

    Display display("Chip-8 Emulator", PHYSICAL_WIDTH_PX, PHYSICAL_HEIGHT_PX,
                    LOGICAL_WIDTH_PX, LOGICAL_HEIGHT_PX, isFullscreen);

    if (!display.isInitialized()) {
        return 1;
    }

    Chip8 chip8;
    if (!chip8.loadROM(filepath)) {
        SDL_Log("Failed to load ROM!");
        return 1;
    }

    const auto MIN_CYCLE_TIME_NS = static_cast<uint64_t>(1.0/clock_hz * 1e9);
    uint64_t CPU_accu_start = SDL_GetTicksNS();
    uint64_t Timers_accu_start = SDL_GetTicksNS();


    while (display.processInput(chip8)) {
        constexpr uint64_t _60HZ_PERIOD_NS = 16666667;

        uint64_t CPU_accu = SDL_GetTicksNS() - CPU_accu_start;
        uint64_t Timers_accu = SDL_GetTicksNS() - Timers_accu_start;


        if (CPU_accu >= MIN_CYCLE_TIME_NS) {

            chip8.cycle();

            CPU_accu_start += CPU_accu;
        }

        if (Timers_accu >= _60HZ_PERIOD_NS) {

            chip8.decrDelayTimer();
            chip8.decrSoundTimer();

            Timers_accu_start += Timers_accu;
        }


        display.render(chip8.getScreenAddr());
    }

    return 0;
}