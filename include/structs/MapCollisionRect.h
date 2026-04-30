#pragma once

struct MapCollisionRect
{
    float area = 0.0f;
    float overlapX = 0.0f;
    float overlapY = 0.0f;
    bool positiveDistX = false;
    bool positiveDistY = false;
};