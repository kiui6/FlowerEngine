#pragma once

#include <Graphics/RenderEngine/RenderState/RenderStateUpdate.h>

#include <memory>

#include <Math/Matrix.h>
#include <Math/Vectors.h>
#include <Graphics/RenderStates/GlobalRenderState.h>

struct GlobalStateUpdate : public RenderStateUpdate
{
    uint32_t canvasWidth, canvasHeight;
    Float3 cameraPosition;

    bool projectionMatrixDirty = false;
    bool cameraPositionDirty = false;

    virtual void Apply(GPUContext& gpu, RenderStateStore& store) override;

};