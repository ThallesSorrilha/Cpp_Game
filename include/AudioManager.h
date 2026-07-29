#pragma once

#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "enums/Audio.h"

class AudioManager
{
public:
    static bool init();
    static void shutdown();

    static bool playSound(const Audio &id, int loops = 0, int channel = -1);
    static bool playMusic(const Audio &id, int loops = -1);
    static void stopMusic();
    static void setSoundVolume(int volume);
    static void setMusicVolume(int volume);

private:
    static bool loadSound(const Audio &id);
    static bool loadMusic(const Audio &id);

    static inline bool initialized = false;
    static inline std::map<Audio, Mix_Chunk *> soundMap;
    static inline std::map<Audio, Mix_Music *> musicMap;
};
