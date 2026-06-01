#pragma once

#include <Graphics/RenderEngine/RenderState/RenderStateUpdate.h>

#include <memory>

#include <Math/Matrix.h>
#include <Math/Vectors.h>
#include <Graphics/RenderStates/UpscaleRenderState.h>

struct UpscaleStateUpdate : public RenderStateUpdate, public StaticallyTyped<MakeID32("UPSC")>
{
    uint32_t gameCanvasWidth, gameCanvasHeight;
    uint32_t viewportWidth, viewportHeight;

    virtual void Apply(RenderStateUpdateContext& ctx) override;
};