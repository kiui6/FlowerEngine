#pragma once

#include <Graphics/RenderEngine/RenderView/RenderResource.h>
#include <Math/Vectors.h>

#include <SDL3/SDL_gpu.h>

#include <Graphics/RenderEngine/RenderUtils.h>

struct OpaqueSpriteRenderElement
{
    // minU, minV, maxU, maxV
    Float4 uv = {0, 0, 1, 1};
    Float4 tint = {1, 1, 1, 1};
    Float2 position = {0, 0};
    Float2 size = {1, 1};
    Float2 pivot = {0.5, 0.5};
    Texture2DResource* texture = nullptr;
    uint32_t depth = 0;
    float rotation = 0;
};

static_assert(sizeof(OpaqueSpriteRenderElement) == 72);