#pragma once

#include <Graphics/RenderEngine/RenderView/RenderElement.h>
#include <Graphics/RenderEngine/RenderView/RenderResource.h>
#include <Math/Vectors.h>

struct ReliefSpriteRenderElement : public RenderElement
{
    ReliefSpriteRenderElement() : RenderElement(RenderPassType::Relief, RenderElementType::Sprite) {}

    Float2 position;
    uint32_t depth;
    Float2 scale;
    float rotation;

    Texture2DResource texture;
    float textureComponentCoefficient = 1 << 8;
    // minU, minV, maxU, maxV
    Float4 uv;

    virtual CompiledRenderElement* CreateCompiledElement() override {return nullptr;}
};

struct AutogenReliefSpriteRenderElement : public RenderElement
{
    AutogenReliefSpriteRenderElement() : RenderElement(RenderPassType::Relief, RenderElementType::Sprite) {}

    Float2 position;
    uint32_t depth;
    Float2 scale;
    float rotation;

    Texture2DResource texture;
    float textureComponentCoefficient = 1 << 8;
    // minU, minV, maxU, maxV
    Float4 uv;

    virtual CompiledRenderElement* CreateCompiledElement() override {return nullptr;}
};