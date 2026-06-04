#pragma once

#include <string_view>
#include <array>

constexpr std::array<std::string_view, 5> VisualLayersArray = {
    "collision",    // 0
    "floor",        // 1
    "decoration",   // 2
    "decoration_2", // 3
    "ceil"};        // 4