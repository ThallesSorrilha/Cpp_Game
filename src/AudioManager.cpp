#include "../include/AudioManager.h"

#include <iostream>
#include <string>
#include <string_view>
#include <utility>

#include "../include/definitions/AudioArray.h"

bool AudioManager::init()
{
    if (initialized)
    {
        return true;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        std::cerr << "Mix_OpenAudio error: " << Mix_GetError() << std::endl;
        return false;
    }

    initialized = true;
    return true;
}

void AudioManager::shutdown()
{
    for (auto &pair : soundMap)
    {
        if (pair.second != nullptr)
        {
            Mix_FreeChunk(pair.second);
        }
    }
    soundMap.clear();

    for (auto &pair : musicMap)
    {
        if (pair.second != nullptr)
        {
            Mix_FreeMusic(pair.second);
        }
    }
    musicMap.clear();

    if (initialized)
    {
        Mix_CloseAudio();
        initialized = false;
    }
}

bool AudioManager::playSound(const Audio &id, int loops, int channel)
{
    if (!initialized)
    {
        std::cerr << "AudioManager not initialized" << std::endl;
        return false;
    }

    if (!loadSound(id))
    {
        return false;
    }

    if (Mix_PlayChannel(channel, soundMap[id], loops) == -1)
    {
        std::cerr << "Mix_PlayChannel error: " << Mix_GetError() << std::endl;
        return false;
    }

    return true;
}

bool AudioManager::playMusic(const Audio &id, int loops)
{
    if (!initialized)
    {
        std::cerr << "AudioManager not initialized" << std::endl;
        return false;
    }

    if (!loadMusic(id))
    {
        return false;
    }

    if (Mix_PlayMusic(musicMap[id], loops) < 0)
    {
        std::cerr << "Mix_PlayMusic error: " << Mix_GetError() << std::endl;
        return false;
    }

    return true;
}

void AudioManager::stopMusic()
{
    if (initialized)
    {
        Mix_HaltMusic();
    }
}

void AudioManager::setSoundVolume(int volume)
{
    if (initialized)
    {
        Mix_Volume(-1, volume);
    }
}

void AudioManager::setMusicVolume(int volume)
{
    if (initialized)
    {
        Mix_VolumeMusic(volume);
    }
}

bool AudioManager::loadSound(const Audio &id)
{
    if (soundMap.find(id) != soundMap.end())
    {
        return true;
    }

    const std::string_view filename = AudioArray[std::to_underlying(id)];
    Mix_Chunk *chunk = Mix_LoadWAV(std::string(filename).c_str());
    if (chunk == nullptr)
    {
        std::cerr << "Mix_LoadWAV error: " << Mix_GetError() << std::endl;
        return false;
    }

    soundMap[id] = chunk;
    return true;
}

bool AudioManager::loadMusic(const Audio &id)
{
    if (musicMap.find(id) != musicMap.end())
    {
        return true;
    }

    const std::string_view filename = AudioArray[std::to_underlying(id)];
    Mix_Music *music = Mix_LoadMUS(std::string(filename).c_str());
    if (music == nullptr)
    {
        std::cerr << "Mix_LoadMUS error: " << Mix_GetError() << std::endl;
        return false;
    }

    musicMap[id] = music;
    return true;
}
