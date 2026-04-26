#pragma once

#include <cstdint>

enum class RenderElementType : uint32_t {
    // Masked sprite, UVs are baked into the Vertex Buffer
    OpaqueSprite,
    // Passes UVs as push constant to avoid buffer updates
    OpaqueSpriteAnimated,
    // Draws height & bump data into the relief buffer for lightning
    Relief
};