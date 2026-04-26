#pragma once

#include <cstdint>

enum class RenderPassType : uint32_t {
    Unknown,
    Opaque,
    Relief,
    Particle,
    Lighting,
    Weather,
    PostProcess,
    Upscale,
    UI
};