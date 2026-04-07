// #include "tests/MemoryTestSet.hpp"
#include "src/Chip8.hpp"
#include <SDL3/SDL.h>

int main(int argc, char* argv[])
{
    constexpr uint16_t PHYSICAL_WIDTH_PX = 1200;
    constexpr uint16_t PHYSICAL_HEIGHT_PX = 600;
    constexpr uint16_t LOGICAL_WIDTH_PX = 64;
    constexpr uint16_t LOGICAL_HEIGHT_PX = 32;

    // constexpr double cpu_clock = 1.0 / 60 * 1000;

    if(!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;

    if (!SDL_CreateWindowAndRenderer("Chip-8 Emulator", PHYSICAL_WIDTH_PX, PHYSICAL_HEIGHT_PX,
        SDL_WINDOW_OPENGL, &window, &renderer)) {
        SDL_Log("SDL couldn't create window and renderer! SDL_Error: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_SetRenderLogicalPresentation(renderer, LOGICAL_WIDTH_PX, LOGICAL_HEIGHT_PX,
        SDL_LOGICAL_PRESENTATION_LETTERBOX);

    SDL_Texture* frame_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING, LOGICAL_WIDTH_PX, LOGICAL_HEIGHT_PX);
    SDL_SetTextureScaleMode(frame_texture, SDL_SCALEMODE_NEAREST);

    Chip8 chip8;
    if (!chip8.loadROM("../test-roms/pong-1-player.ch8")) {
        return 1;
    }

    bool running = true;

    while (running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
                break;
            }

            if (event.type == SDL_EVENT_KEY_DOWN) {
                switch (event.key.scancode) {
                    case SDL_SCANCODE_1:
                        chip8.setKeyPress(0x1, true);
                        break;
                    case SDL_SCANCODE_2:
                        chip8.setKeyPress(0x2, true);
                        break;
                    case SDL_SCANCODE_3:
                        chip8.setKeyPress(0x3, true);
                        break;
                    case SDL_SCANCODE_4:
                        chip8.setKeyPress(0xC, true);
                        break;
                    case SDL_SCANCODE_Q:
                        chip8.setKeyPress(0x4, true);
                        break;
                    case SDL_SCANCODE_W:
                        chip8.setKeyPress(0x5, true);
                        break;
                    case SDL_SCANCODE_E:
                        chip8.setKeyPress(0x6, true);
                        break;
                    case SDL_SCANCODE_R:
                        chip8.setKeyPress(0xD, true);
                        break;
                    case SDL_SCANCODE_A:
                        chip8.setKeyPress(0x7, true);
                        break;
                    case SDL_SCANCODE_S:
                        chip8.setKeyPress(0x8, true);
                        break;
                    case SDL_SCANCODE_D:
                        chip8.setKeyPress(0x9, true);
                        break;
                    case SDL_SCANCODE_F:
                        chip8.setKeyPress(0xE, true);
                        break;
                    case SDL_SCANCODE_Z:
                        chip8.setKeyPress(0xA, true);
                        break;
                    case SDL_SCANCODE_X:
                        chip8.setKeyPress(0x0, true);
                        break;
                    case SDL_SCANCODE_C:
                        chip8.setKeyPress(0xB, true);
                        break;
                    case SDL_SCANCODE_V:
                        chip8.setKeyPress(0xF, true);
                        break;
                    default: ;
                }
            }

            if (event.type == SDL_EVENT_KEY_UP) {
                switch (event.key.scancode) {
                    case SDL_SCANCODE_1:
                        chip8.setKeyPress(0x1, false);
                        break;
                    case SDL_SCANCODE_2:
                        chip8.setKeyPress(0x2, false);
                        break;
                    case SDL_SCANCODE_3:
                        chip8.setKeyPress(0x3, false);
                        break;
                    case SDL_SCANCODE_4:
                        chip8.setKeyPress(0xC, false);
                        break;
                    case SDL_SCANCODE_Q:
                        chip8.setKeyPress(0x4, false);
                        break;
                    case SDL_SCANCODE_W:
                        chip8.setKeyPress(0x5, false);
                        break;
                    case SDL_SCANCODE_E:
                        chip8.setKeyPress(0x6, false);
                        break;
                    case SDL_SCANCODE_R:
                        chip8.setKeyPress(0xD, false);
                        break;
                    case SDL_SCANCODE_A:
                        chip8.setKeyPress(0x7, false);
                        break;
                    case SDL_SCANCODE_S:
                        chip8.setKeyPress(0x8, false);
                        break;
                    case SDL_SCANCODE_D:
                        chip8.setKeyPress(0x9, false);
                        break;
                    case SDL_SCANCODE_F:
                        chip8.setKeyPress(0xE, false);
                        break;
                    case SDL_SCANCODE_Z:
                        chip8.setKeyPress(0xA, false);
                        break;
                    case SDL_SCANCODE_X:
                        chip8.setKeyPress(0x0, false);
                        break;
                    case SDL_SCANCODE_C:
                        chip8.setKeyPress(0xB, false);
                        break;
                    case SDL_SCANCODE_V:
                        chip8.setKeyPress(0xF, false);
                        break;
                    default: ;
                }
            }
        }

        // SDL_Delay(static_cast<uint32_t>(cpu_clock));
        SDL_Delay(16);

        chip8.cycle();

        SDL_UpdateTexture(frame_texture, nullptr, chip8.getScreenAddr(), LOGICAL_WIDTH_PX * 4);
        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, frame_texture, nullptr, nullptr);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(frame_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
