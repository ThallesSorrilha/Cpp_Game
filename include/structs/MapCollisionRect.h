#pragma once

struct MapCollisionRect
{
    int x = 0;
    int y = 0;
    float area = 0.0f;
    float overlapX = 0.0f;
    float overlapY = 0.0f;
    bool positiveDistX = false;
    bool positiveDistY = false;
};