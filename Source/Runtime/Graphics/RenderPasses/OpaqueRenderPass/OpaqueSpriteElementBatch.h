#pragma once

#include <array>
#include <vector>

#include <Math/Vectors.h>
#include <Graphics/RenderEngine/RenderUtils.h>
#include <Graphics/RenderEngine/RenderConstants.h>
#include <SDL3/SDL_gpu.h>

struct OpaqueSpriteElementBatch
{
    struct alignas(16) GPUBufferData {
        Float4 uv;
        Float4 tint;
        Float2 position;
        Float2 size;
        Float2 pivot;
        uint32_t depth;
        float rotation;
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

static_assert(sizeof(OpaqueSpriteElementBatch::GPUBufferData) == 64, "Sprite data size mismatch");