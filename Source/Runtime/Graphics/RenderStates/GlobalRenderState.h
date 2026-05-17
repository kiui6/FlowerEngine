#pragma once

#include <Graphics/RenderEngine/RenderState/RenderState.h>

#include <Math/Matrix.h>

struct GlobalRenderState : public RenderState
{
    GlobalRenderState(GPUContext& gpu);

    static ID32 StaticType() {return MakeID32("GLOB");} 

    Float4x4 projectionMatrix;
    uint32_t canvasWidth, canvasHeight;
};