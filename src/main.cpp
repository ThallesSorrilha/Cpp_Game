#include "GameManager/GameManager.h"
#include <iostream>

int main(int argc, char *argv[])
{
    GameManager gameManager;
    if (!gameManager.init())
    {
        std::cerr << "Init failed\n";
        return 1;
    }
    gameManager.run();
    gameManager.shutdown();
    return 0;
}