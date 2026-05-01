#pragma once

#include <cstdint>

enum class RenderPassType : uint32_t {
    Opaque,
    Relief,
    Particle,
    Lighting,
    Weather,
    PostProcess,
    Upscale,
    UI,
    MAX
};