#pragma once

#include <Graphics/RenderEngine/RenderState/RenderStateUpdate.h>

#include <memory>

#include <Math/Matrix.h>
#include <Math/Vectors.h>
#include <Graphics/RenderStates/GlobalRenderState.h>

struct GlobalStateUpdate : public RenderStateUpdate, public StaticallyTyped<MakeID32("GLOB")>
{
    uint32_t canvasWidth, canvasHeight;
    Float3 cameraPosition;

    bool canvasDirty = false;

    virtual void Apply(RenderStateUpdateContext& ctx) override;

protected:
    SDL_GPUTransferBuffer* transferBuffer = nullptr;
};