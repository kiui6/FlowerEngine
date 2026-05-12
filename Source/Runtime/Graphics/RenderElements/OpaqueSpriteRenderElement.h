#pragma once

#include <Graphics/RenderEngine/RenderView/RenderElement.h>
#include <Graphics/RenderEngine/RenderView/CompiledRenderElement.h>
#include <Graphics/RenderEngine/RenderView/RenderResource.h>
#include <Math/Vectors.h>

#include <SDL3/SDL_gpu.h>

#include <Graphics/RenderEngine/RenderUtils.h>

struct OpaqueSpriteRenderElement : public RenderElement
{
    OpaqueSpriteRenderElement() : RenderElement(RenderPassType::Opaque, RenderElementType::Sprite) {}

    Float2 position;
    uint32_t depth;
    Float2 scale;
    float rotation;
    Float3 tint = {1, 1, 1};

    Texture2DResource* texture = nullptr;
    // minU, minV, maxU, maxV
    Float4 uv;
};

struct CompiledOpaqueSpriteRenderElement : public CompiledRenderElement
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
    constexpr static size_t elementSize = sizeof(CompiledOpaqueSpriteRenderElement::GPUBufferData);

    // Assembly
    std::vector<GPUBufferData> assembly;

    // Compiled resources
    std::array<SDL_GPUBuffer*, FRAMES_IN_FLIGHT> uniformBuffer = {};
    std::array<SDL_GPUTransferBuffer*, FRAMES_IN_FLIGHT> transferBuffer = {};
    std::array<size_t, FRAMES_IN_FLIGHT> bufferSize = {};
};