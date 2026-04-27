#include "../include/GameScene.h"

GameScene::GameScene(const Config &config)
    : initialized(config.initialized),
      paused(config.paused)
{
}