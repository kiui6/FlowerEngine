#pragma once

#include <Graphics/RenderEngine/RenderState/RenderStateUpdate.h>

#include <memory>

#include <Math/Matrix.h>
#include <Math/Vectors.h>
#include <Graphics/RenderStates/UpscaleRenderState.h>

struct UpscaleStateUpdate : public RenderStateUpdate, public StaticallyTyped<MakeID32("UPSC")>
{
    inline void SetGameCanvas(uint32_t width, uint32_t height) {
        gameCanvasWidth = width;
        gameCanvasHeight = height;
        gameCanvasDirty = true;
    }

    inline void SetViewport(uint32_t width, uint32_t height) {
        viewportWidth = width;
        viewportHeight = height;
        viewportDirty = true;
    }

    virtual void Apply(RenderStateUpdateContext& ctx) override;
protected:
    uint32_t gameCanvasWidth, gameCanvasHeight;
    bool gameCanvasDirty = false;
    uint32_t viewportWidth, viewportHeight;
    bool viewportDirty = false;
};