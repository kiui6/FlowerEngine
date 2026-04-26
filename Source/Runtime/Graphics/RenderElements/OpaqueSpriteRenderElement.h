#pragma once

#include <Graphics/RenderEngine/RenderView/RenderElement.h>
#include <Graphics/RenderEngine/RenderView/RenderResource.h>
#include <Math/Vectors.h>

struct OpaqueSpriteRenderElement : public RenderElement
{
    OpaqueSpriteRenderElement() : RenderElement(RenderElementType::OpaqueSprite) {}

    Float2 position;
    uint32_t depth;
    float scale;
    float rotation;

    Texture2DResource texture;
    // minU, minV, maxU, maxV
    Float4 uv;

    virtual CompiledRenderElement* CreateCompiledElement();
};