#pragma once

#include <Graphics/RenderEngine/RenderState/RenderState.h>
#include <Graphics/RenderEngine/RenderConstants.h>

#include <Math/Vectors.h>

#include <array>

struct UpscaleRenderState : public RenderState, public StaticallyTyped<MakeID32("UPSC")>
{
    UpscaleRenderState(GPUContext& gpu) : RenderState(gpu) {}

    uint32_t gameCanvasWidth = 0, gameCanvasHeight = 0;
    uint32_t viewportWidth = 0, viewportHeight = 0;

    Float2 compensationCalculation = {1, 1};
};