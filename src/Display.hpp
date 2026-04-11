#ifndef CHIP8_EMULATOR_DISPLAY_HPP
#define CHIP8_EMULATOR_DISPLAY_HPP

#include "Chip8.hpp"
#include <SDL3/SDL.h>
#include <string_view>

using namespace std;

class Display {
public:
    Display(string_view title, uint16_t physicalWidth, uint16_t physicalHeight,
                uint16_t logicalWidth, uint16_t logicalHeight, bool enableFullscreen);
    ~Display();

    bool isInitialized();
    bool processInput(Chip8& chip8);
    void render(const void* pixels) const;

private:
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;
    uint16_t logicalWidth;
    bool initSuccessful;
};


#endif //CHIP8_EMULATOR_DISPLAY_HPP