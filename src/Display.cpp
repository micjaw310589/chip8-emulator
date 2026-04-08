#include "Display.hpp"

Display::Display(const string_view title, const uint16_t physicalWidth, const uint16_t physicalHeight,
            const uint16_t logicalWidth, const uint16_t logicalHeight)
        : logicalWidth(logicalWidth)
        , initSuccessful(false)
{

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL could not initialize! SDL_Error: %s", SDL_GetError());
        return;
    }

    if (!SDL_CreateWindowAndRenderer(title.data(), physicalWidth, physicalHeight,
        SDL_WINDOW_OPENGL, &window, &renderer)) {
        SDL_Log("SDL couldn't create window and renderer! SDL_Error: %s", SDL_GetError());
        return;
    }

    SDL_SetRenderLogicalPresentation(renderer, logicalWidth, logicalHeight,
        SDL_LOGICAL_PRESENTATION_LETTERBOX);

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING, logicalWidth, logicalHeight);

    if (!texture) {
        SDL_Log("SDL couldn't create texture! SDL_Error: %s", SDL_GetError());
        return;
    }

    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    initSuccessful = true;
}

Display::~Display()
{
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

bool Display::isInitialized()
{
    return initSuccessful;
}

// Processes events and updates keypad state.
// Returns `false` if user closed the window.
bool Display::processInput(Chip8& chip8)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            return false;
        }

        if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
            bool isPressed = (event.type == SDL_EVENT_KEY_DOWN);

            switch (event.key.scancode) {
                case SDL_SCANCODE_1: chip8.setKeyPress(0x1, isPressed); break;
                case SDL_SCANCODE_2: chip8.setKeyPress(0x2, isPressed); break;
                case SDL_SCANCODE_3: chip8.setKeyPress(0x3, isPressed); break;
                case SDL_SCANCODE_4: chip8.setKeyPress(0xC, isPressed); break;
                case SDL_SCANCODE_Q: chip8.setKeyPress(0x4, isPressed); break;
                case SDL_SCANCODE_W: chip8.setKeyPress(0x5, isPressed); break;
                case SDL_SCANCODE_E: chip8.setKeyPress(0x6, isPressed); break;
                case SDL_SCANCODE_R: chip8.setKeyPress(0xD, isPressed); break;
                case SDL_SCANCODE_A: chip8.setKeyPress(0x7, isPressed); break;
                case SDL_SCANCODE_S: chip8.setKeyPress(0x8, isPressed); break;
                case SDL_SCANCODE_D: chip8.setKeyPress(0x9, isPressed); break;
                case SDL_SCANCODE_F: chip8.setKeyPress(0xE, isPressed); break;
                case SDL_SCANCODE_Z: chip8.setKeyPress(0xA, isPressed); break;
                case SDL_SCANCODE_X: chip8.setKeyPress(0x0, isPressed); break;
                case SDL_SCANCODE_C: chip8.setKeyPress(0xB, isPressed); break;
                case SDL_SCANCODE_V: chip8.setKeyPress(0xF, isPressed); break;
                default: break;
            }
        }
    }
    return true;
}

// Draws frame buffer on the screen
void Display::render(const void* pixels) const
{
    SDL_UpdateTexture(texture, nullptr, pixels, logicalWidth * 4);
    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}
