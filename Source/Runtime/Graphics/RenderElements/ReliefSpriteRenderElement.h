#pragma once

#include <Graphics/RenderEngine/RenderView/RenderResource.h>
#include <Math/Vectors.h>

struct ReliefSpriteRenderElement
{
    Float2 position;
    uint32_t depth;
    Float2 scale;
    float rotation;

    Texture2DResource* texture = nullptr;
    float textureComponentCoefficient = 1 << 8;
    // minU, minV, maxU, maxV
    Float4 uv;
};

struct AutogenReliefSpriteRenderElement
{
    Float2 position;
    uint32_t depth;
    Float2 scale;
    float rotation;

    Texture2DResource texture;
    float textureComponentCoefficient = 1 << 8;
    // minU, minV, maxU, maxV
    Float4 uv;
};