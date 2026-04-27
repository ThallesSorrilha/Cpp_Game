#pragma once

#include <SDL2/SDL.h>

class Timer
{

public:
    Timer() = default;
    ~Timer() = default;

    void setTimer(float seconds)
    {
        targetTime = static_cast<Uint32>(seconds * 1000) + SDL_GetTicks();
    }

    bool isEnd()
    {
        return SDL_GetTicks() >= targetTime;
    }

    bool isIn()
    {
        return SDL_GetTicks() < targetTime;
    }

    void reset()
    {
        targetTime = 0;
    }

private:
    Uint32 targetTime = 0;
};
