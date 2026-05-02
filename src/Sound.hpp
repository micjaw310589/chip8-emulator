#ifndef CHIP8_EMULATOR_SOUND_HPP
#define CHIP8_EMULATOR_SOUND_HPP

#include <SDL3/SDL_audio.h>
#include <cstdint>

class Sound
{
private:
    SDL_AudioStream* stream = nullptr;
    SDL_AudioSpec spec;
    float samples[512];
    int current_sine_sample;

public:
    Sound();
    ~Sound(); // Warto dodać destruktor do czyszczenia zasobów SDL

    bool bufferSamples();
    void playSound() const;
    void pauseSound() const;
};

#endif //CHIP8_EMULATOR_SOUND_HPP