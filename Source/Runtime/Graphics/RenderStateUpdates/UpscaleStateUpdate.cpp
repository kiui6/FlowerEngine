#include "UpscaleStateUpdate.h"

#include <glm/gtc/matrix_transform.hpp>

#include <Graphics/RenderEngine/RenderUtils.h>

#include <Log/Log.h>

void UpscaleStateUpdate::Apply(RenderStateUpdateContext &ctx)
{
    UpscaleRenderState& state = ctx.store.GetMutable<UpscaleRenderState>();

    if(gameCanvasDirty ) {
        if(gameCanvasWidth == 0 || gameCanvasHeight == 0){
            LOG(Assert, LogUpscaleStateUpdate, "Tried updating upscale render state, but one or more of game canvas dimensions is 0.");
            viewportDirty = false;
            gameCanvasDirty = false;
            return;
        }

        state.gameCanvasWidth = gameCanvasWidth;
        state.gameCanvasHeight = gameCanvasHeight;

        gameCanvasDirty = false;
    }

    if(viewportDirty ) {
        if(viewportWidth == 0 || viewportHeight == 0){
            LOG(Assert, LogUpscaleStateUpdate, "Tried updating upscale render state, but one or more of viewport dimensions is 0.");
            viewportDirty = false;
            gameCanvasDirty = false;
            return;
        }

        state.viewportWidth = viewportWidth;
        state.viewportHeight = viewportHeight;

        viewportDirty = false;
    }

    float CanvasAspect = ((float)state.gameCanvasWidth / (float)state.gameCanvasHeight);
    float ViewportAspect =  (float)state.viewportWidth / (float)state.viewportHeight;
    
    if(ViewportAspect > CanvasAspect) {
        // Compensate for lack of Y
        state.compensationCalculation.x = 1;
        // kY = Xc / ((Xo/Yo) * Yc)
        state.compensationCalculation.y = (float)state.viewportWidth / (CanvasAspect * (float)state.viewportHeight);
    } else if(ViewportAspect < CanvasAspect) {
        // Compensate for lack of X
        state.compensationCalculation.y = 1;
        // kX = ((Xo/Yo) * Yc) / Xc
        state.compensationCalculation.x = (CanvasAspect * (float)state.viewportHeight) / (float)state.viewportWidth;
    } else {
        state.compensationCalculation.y = 1;
        state.compensationCalculation.x = 1;
    }

    float widthScaleFactor = (float)state.gameCanvasWidth / (float)state.viewportWidth;
    float heightScaleFactor = (float)state.gameCanvasHeight / (float)state.viewportHeight;

    if(std::min(widthScaleFactor, heightScaleFactor) > 1.f) {
        state.compensationCalculation.y *= std::max(widthScaleFactor, heightScaleFactor);
        state.compensationCalculation.x *= std::max(widthScaleFactor, heightScaleFactor);
    }
    
    state.NotifyChanged();
}