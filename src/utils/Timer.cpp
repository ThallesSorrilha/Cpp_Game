#include "Timer.h"

Timer::Timer() {}
Timer::~Timer() {}

void Timer::setTimer(float seconds)
{
    targetTime = static_cast<Uint32>(seconds * 1000) + SDL_GetTicks();
}

bool Timer::isEnd()
{
    return SDL_GetTicks() >= targetTime;
}

bool Timer::isIn()
{
    return SDL_GetTicks() < targetTime;
}

void Timer::reset()
{
    targetTime = 0;
}
