#pragma once

#include <bit>
#include <cstdint>
#include <stdexcept>
#include <utility>

// até 32
enum class LayerID
{
    World,
    Player,
    Enemy,
    PlayerAttack,
    Last
};

namespace LayerUtils {

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

    constexpr LayerID getLayer(std::uint32_t mask)
    {
        if (!std::has_single_bit(mask))
        {
            throw std::invalid_argument("invalid mask");
        }

        const auto layerIndex = static_cast<int>(std::countr_zero(mask));
        constexpr int maxLayer = std::to_underlying(LayerID::Last);

        if (layerIndex > maxLayer)
        {
            throw std::out_of_range("layer out of enum LayerID range");
        }

        return static_cast<LayerID>(layerIndex);
    }
}