#pragma once

#include <utility>

// até 32
enum class LayerID
{
    World,
    Player,
    Enemy
};

constexpr std::uint32_t toMask(LayerID layer)
{
    return (1u << std::to_underlying(layer));
}

constexpr bool hasLayer(std::uint32_t value, LayerID layer)
{
    return (value & toMask(layer)) != 0u;
}

constexpr bool hasLayer(std::uint32_t mask, std::uint32_t layer)
{
    return (mask & layer) != 0u;
}