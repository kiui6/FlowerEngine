#pragma once

#include <Graphics/RenderEngine/RenderView/RenderElement.h>
#include <Graphics/RenderEngine/RenderView/RenderResource.h>
#include <Math/Vectors.h>

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

    virtual CompiledRenderElement* CreateCompiledElement() override {return nullptr;}
};