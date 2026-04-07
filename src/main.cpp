#include <iostream>

#include "GameManager/GameManager.h"

int main(int argc, char *argv[])
{
    if (!GameManager::init())
    {
        std::cerr << "Init failed\n";
        return 1;
    }
    GameManager::run();
    GameManager::shutdown();
    return 0;
}