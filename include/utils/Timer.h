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

    bool isEnd() const
    {
        return SDL_GetTicks() >= targetTime;
    }

    bool isIn() const
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
