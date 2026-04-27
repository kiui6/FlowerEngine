#pragma once

#include <cstdint>

enum class RenderElementType : uint32_t {
    Unknown,
    // Masked sprite, UVs are baked into the Uniform Buffer for instanced rendering
    Sprite,
    // Passes UVs as push constant to avoid buffer updates, not instanced
    SpriteAnimated,
    // Allows rendering of particle systems
    Particles,
    // Draws tilemap's atlas layer instanced
    TilemapAtlasLayer,
    // Draws tilemap's procedural layer using specified shader
    TilemapProceduralLayer,
};