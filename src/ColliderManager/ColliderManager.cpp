#include "ColliderManager.h"

#include <cstddef>
#include <vector>

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
    const std::vector<MapCollisionRect> collisions = detectCollisionAgainstTileMap(position, colliderBox, tileMap);

    if (collisions.empty())
    {
        return;
    }

    std::cout << " -------- Ciclo: " << SDL_GetTicks() << "  -------- " << std::endl;

    bool isYCorrectec = false;
    bool isXCorrectec = false;

    for (const MapCollisionRect &rect : collisions)
    {
        std::cout << " --> Bloco: [" << rect.x << "," << rect.y << "] - " << rect.w << "x" << rect.h << std::endl;
        if (isXCorrectec && isYCorrectec)
        {
            break;
        }

        //  Distância do meio de uma colliderBox para o meio de retângulo
        std::cout << "position: [" << colliderX << "," << colliderY << "]" << std::endl;
        float distX = (colliderX + size.x / 2) - (rect.x + rect.w / 2);
        std::cout << "distX: " << distX << std::endl;
        float distY = (colliderY + size.y / 2) - (rect.y + rect.h / 2);
        std::cout << "distY: " << distY << std::endl;
        //  Metades combinadas: distância limite de colisão
        float halfW = size.x / 2 + rect.w / 2;
        std::cout << "halfW: " << halfW << std::endl;
        float halfH = size.y / 2 + rect.h / 2;
        std::cout << "halfH: " << halfH << std::endl;

        // calculando sobreposição: distância limítrofe de colisão (a partir do centro) - distância real dos centros
        float overlapX = halfW - std::abs(distX);
        std::cout << "overlapX: " << overlapX << std::endl;
        float overlapY = halfH - std::abs(distY);
        std::cout << "overlapY: " << overlapY << std::endl;
        //  procura a menor sobreposição

        // Se sobreposição for negativa, não houve toque
        if (overlapX <= 0 || overlapY <= 0)
        {
            return;
        }
        
        // resolver pela menor sobreposição
        if (overlapY < overlapX)
        {
            if (isYCorrectec)
            {
                continue;
            }

            if (distY > 0)
            {
                // colisão ACIMA (em relação ao collisionBox)
                position.y += overlapY;
                std::cout << "colisão ACIMA - newPosition = " << position << std::endl;
            }
            else
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
            else
            {
                // colisão DIREITA
                position.x -= overlapX;
                std::cout << "colisão DIREITA - newPosition = " << position << std::endl;
            }
            isXCorrectec = true;
        }
    }
}

std::vector<MapCollisionRect> ColliderManager::detectCollisionAgainstTileMap(
    const Vector2D &position,
    const ColliderBox &colliderBox,
    const TileMap &tileMap)
{
    std::vector<MapCollisionRect> collisions;

    if (!colliderBox.isEnabled() || colliderBox.getSize().x <= 0.0f || colliderBox.getSize().y <= 0.0f)
    {
        return collisions;
    }

    const float xPosition = position.x + colliderBox.getOffset().x;
    const float yPosition = position.y + colliderBox.getOffset().y;
    constexpr float epsilon = 0.0001f;

    const int minTileX = static_cast<int>(std::floor(xPosition));
    const int minTileY = static_cast<int>(std::floor(yPosition));
    const int maxTileX = static_cast<int>(std::floor(xPosition + colliderBox.getSize().x - epsilon));
    const int maxTileY = static_cast<int>(std::floor(yPosition + colliderBox.getSize().y - epsilon));

    const int rangeWidth = maxTileX - minTileX + 1;
    const int rangeHeight = maxTileY - minTileY + 1;

    if (rangeWidth <= 0 || rangeHeight <= 0)
    {
        return collisions;
    }

    const std::size_t totalTiles = static_cast<std::size_t>(rangeWidth) * static_cast<std::size_t>(rangeHeight);
    std::vector<unsigned char> solidTiles(totalTiles, 0);
    std::vector<unsigned char> usedTiles(totalTiles, 0);

    // tentar estruturar melhor essa função no código, ao invés de arrow function praticamente
    const auto indexOf = [rangeWidth](int localX, int localY) -> std::size_t
    {
        return static_cast<std::size_t>(localY) * static_cast<std::size_t>(rangeWidth) + static_cast<std::size_t>(localX);
    };

    // preenche solidTiles (para representar a colisão)
    for (int localY = 0; localY < rangeHeight; ++localY)
    {
        for (int localX = 0; localX < rangeWidth; ++localX)
        {
            const int tileX = minTileX + localX;
            const int tileY = minTileY + localY;
            if (tileMap.isCollisionTile(tileX, tileY))
            {
                solidTiles[indexOf(localX, localY)] = 1;
            }
        }
    }

    // Greedy meshing: cria retângulos máximos sem sobreposição dentro da área consultada.
    // percorre linhas
    for (int localY = 0; localY < rangeHeight; ++localY)
    {
        // percorre colunas
        for (int localX = 0; localX < rangeWidth; ++localX)
        {
            // pega o index com base na linha e coluna
            const std::size_t originIndex = indexOf(localX, localY);

            if (solidTiles[originIndex] == 0 || usedTiles[originIndex] != 0)
            {
                continue;
            }

            // começa de um tile colisível e tenta expandir a largura do retângulo em direção à direita
            int rectWidth = 0;
            while (localX + rectWidth < rangeWidth)
            {
                const std::size_t currentIndex = indexOf(localX + rectWidth, localY);
                if (solidTiles[currentIndex] == 0 || usedTiles[currentIndex] != 0)
                {
                    break;
                }
                ++rectWidth;
            }
            // por fim define a altura como 1
            int rectHeight = 1;

            // percorre verticalmente os tiles, aumentando a altura do retângulo
            bool canExpand = true;
            while (canExpand && (localY + rectHeight < rangeHeight))
            {
                // percorre uma linha do retângulo, verificando se são colisíveis
                for (int dx = 0; dx < rectWidth; ++dx)
                {
                    // vai obtendo o índice de cada tile do retângulo
                    const std::size_t currentIndex = indexOf(localX + dx, localY + rectHeight);
                    if (solidTiles[currentIndex] == 0 || usedTiles[currentIndex] != 0)
                    {
                        canExpand = false;
                        break;
                    }
                }

                // expande altura
                if (canExpand)
                {
                    ++rectHeight;
                }
            }

            // percorre toda a área do retângulo, dizendo que os tiles já foram usados
            for (int dy = 0; dy < rectHeight; ++dy)
            {
                for (int dx = 0; dx < rectWidth; ++dx)
                {
                    usedTiles[indexOf(localX + dx, localY + dy)] = 1;
                }
            }

            // adiciona retângulo à lista
            collisions.push_back(MapCollisionRect{
                .x = static_cast<float>(minTileX + localX),
                .y = static_cast<float>(minTileY + localY),
                .w = static_cast<float>(rectWidth),
                .h = static_cast<float>(rectHeight)});
        }
    }

    return collisions;
}