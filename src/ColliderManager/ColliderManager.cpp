#include "ColliderManager.h"

#include "../ColliderBox/ColliderBox.h"
#include "../TileMap/TileMap.h"

namespace
{
    constexpr float kSeparationEpsilon = 0.00f;
}

void ColliderManager::resolveMovementAgainstTileMap(
    Vector2D &position,
    const ColliderBox &colliderBox,
    const TileMap &tileMap)
{
    if (!colliderBox.isEnabled())
    {
        return;
    }

    const Vector2D &offset = colliderBox.getOffset();
    const Vector2D &size = colliderBox.getSize();
    const float colliderX = position.x + offset.x;
    const float colliderY = position.y + offset.y;
    const std::vector<TileMap::CollisionTileInfo> collisions =
        tileMap.getCollidingTilesAtWorld(colliderX, colliderY, size.x, size.y);

    if (collisions.empty())
    {
        return;
    }

    std::cout << " -------- Ciclo: " << SDL_GetTicks() << "  -------- " << std::endl;

    bool isYCorrectec = false;
    bool isXCorrectec = false;

    for (const TileMap::CollisionTileInfo &tile : collisions)
    {
        std::cout << " - Bloco: [" << tile.tileX << "," << tile.tileY << "] - " << std::endl;

        if (isXCorrectec && isYCorrectec)
        {
            break;
        }

        std::cout << "position: [" << colliderX << "," << colliderY << "]" << std::endl;
        // Distância no eixo X
        float distX = (colliderX + size.x / 2) - (tile.tileX + 0.5);
        std::cout << "distX: " << distX << std::endl;
        // Distância no eixo Y
        float distY = (colliderY + size.y / 2) - (tile.tileY + 0.5);
        std::cout << "distY: " << distY << std::endl;
        // Metades combinadas
        float halfW = size.x / 2 + 0.5;
        std::cout << "halfW: " << halfW << std::endl;
        float halfH = size.y / 2 + 0.5;
        std::cout << "halfH: " << halfH << std::endl;

        // calcular sobreposição atual
        float overlapX = halfW - std::abs(distX);
        std::cout << "overlapX: " << overlapX << std::endl;
        float overlapY = halfH - std::abs(distY);
        std::cout << "overlapY: " << overlapY << std::endl;
        // procura a menor sobreposição
        if (overlapX > overlapY)
        {
            if (isYCorrectec)
            {
                continue;
            }

            if (distY > 0)
            {
                // colisão ACIMA
                position.y += overlapY;
                std::cout << "colisão ACIMA - newPosition = " << position << std::endl;
            }
            else if (distY < 0)
            {
                // colisão ABAIXO
                position.y -= overlapY;
                std::cout << "colisão ABAIXO - newPosition = " << position << std::endl;
            }
            isYCorrectec = true;
        }
        else
        {
            if (isXCorrectec)
            {
                continue;
            }

            if (distX > 0)
            {
                // colisão ESQUERDA
                position.x += overlapX;
                std::cout << "colisão ESQUERDA - newPosition = " << position << std::endl;
            }
            else if (distX < 0)
            {
                // colisão DIREITA
                position.x -= overlapX;
                std::cout << "colisão DIREITA - newPosition = " << position << std::endl;
            }
            isXCorrectec = true;
        }
    }
}