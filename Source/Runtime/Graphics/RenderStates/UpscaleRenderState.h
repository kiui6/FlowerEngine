#pragma once

#include <Graphics/RenderEngine/RenderState/RenderState.h>
#include <Graphics/RenderEngine/RenderConstants.h>

#include <Math/Vectors.h>

#include <array>

struct UpscaleRenderState : public RenderState, public StaticallyTyped<MakeID32("UPSC")>
{
    UpscaleRenderState(GPUContext& gpu) : RenderState(gpu) {}

   Float2 compensationCalculation = {1, 1};
};