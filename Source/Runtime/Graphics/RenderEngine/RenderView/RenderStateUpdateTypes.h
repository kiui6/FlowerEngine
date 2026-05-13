#pragma once

#include <cstdint>

enum class RenderStateUpdateType : uint32_t {
    Unknown,
    // Updates tilemap's global information
    TilemapGlobals,
    // Updates a pointer to host Debug window to render
    DebugUI,
    // Updates a pointer to a retainer mode UI widget to render
    UI,
};