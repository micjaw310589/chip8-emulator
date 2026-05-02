#include "Sound.hpp"
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>

Sound::Sound()
    : spec{}
    , samples{}
    , current_sine_sample(0)
{
    if (!SDL_Init(SDL_INIT_AUDIO)) {
        SDL_Log("SDL could not initialize! SDL_Error: %s", SDL_GetError());
        return;
    }

    spec.format = SDL_AUDIO_F32;
    spec.channels = 1;
    spec.freq = 8000;

    stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, nullptr, nullptr);
    if (!stream) {
        SDL_Log("Couldn't create audio stream: %s", SDL_GetError());
        return;
    }
}

Sound::~Sound()
{
    if (stream) {
        SDL_DestroyAudioStream(stream);
    }
    SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

bool Sound::bufferSamples()
{
    const int minimum_audio = (spec.freq * static_cast<int>(sizeof(float))) / 2;

    if (SDL_GetAudioStreamQueued(stream) < minimum_audio) {
        /* generate a 440Hz pure tone */
        for (int i = 0; i < SDL_arraysize(samples); i++) {
            constexpr int freq = 440;
            const float phase = static_cast<float>(current_sine_sample) * freq / static_cast<float>(spec.freq);
            samples[i] = SDL_sinf(phase * 2.0f * SDL_PI_F);
            current_sine_sample++;
        }

        /* wrapping around to avoid floating-point errors */
        current_sine_sample %= spec.freq;

        SDL_PutAudioStreamData(stream, samples, sizeof(samples));

        return true;
    }

    return false;
}

void Sound::playSound() const
{
    SDL_ResumeAudioStreamDevice(stream);
}

void Sound::pauseSound() const
{
    SDL_PauseAudioStreamDevice(stream);
}
