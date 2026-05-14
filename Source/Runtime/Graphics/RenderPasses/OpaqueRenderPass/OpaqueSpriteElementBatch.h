#pragma once

#include <array>
#include <vector>

#include <Math/Vectors.h>
#include <Graphics/RenderEngine/RenderUtils.h>
#include <Graphics/RenderEngine/RenderConstants.h>
#include <SDL3/SDL_gpu.h>

struct OpaqueSpriteElementBatch
{
    struct GPUBufferData {
        Float2 position;
        uint32_t depth;
        Float2 scale;
        float rotation;
        Float3 tint;
        Float4 uv;
    };
public:
    constexpr static size_t elementSize = sizeof(GPUBufferData);

    // Assembly
    std::vector<GPUBufferData> assembly;
    bool bIsDirty = false;

    // Compiled resources
    std::array<SDL_GPUBuffer*, FRAMES_IN_FLIGHT> uniformBuffer = {};
    std::array<SDL_GPUTransferBuffer*, FRAMES_IN_FLIGHT> transferBuffer = {};
    std::array<size_t, FRAMES_IN_FLIGHT> bufferSize = {};
};