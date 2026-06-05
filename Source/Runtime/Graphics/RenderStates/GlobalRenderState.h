#pragma once

#include <Graphics/RenderEngine/RenderState/RenderState.h>
#include <Graphics/RenderEngine/RenderConstants.h>

#include <Math/Matrix.h>
#include <Math/Vectors.h>

#include <array>



struct GlobalRenderState : public RenderState, public StaticallyTyped<MakeID32("GLOB")>
{
    GlobalRenderState(GPUContext& gpu);
    ~GlobalRenderState();

    uint32_t canvasWidth, canvasHeight;
};