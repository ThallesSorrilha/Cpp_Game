#pragma once

#include <SDL2/SDL.h>

class Timer
{

public:
    Timer();
    ~Timer();

    void setTimer(float seconds);
    bool isEnd();
    bool isIn();
    void reset();

private:
    Uint32 targetTime = 0;
};
