#pragma once

#include <cstdint>

enum class RenderStateUpdateType : uint32_t {
    Unknown,
    // Updates tilemap's global information
    TilemapGlobals,
};