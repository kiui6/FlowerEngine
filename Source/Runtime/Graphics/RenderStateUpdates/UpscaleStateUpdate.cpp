#include "UpscaleStateUpdate.h"

#include <glm/gtc/matrix_transform.hpp>

#include <Graphics/RenderEngine/RenderUtils.h>

#include <Log/Log.h>

void UpscaleStateUpdate::Apply(RenderStateUpdateContext& ctx)
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

    float OriginalViewportAspect = ((float)state.gameCanvasWidth / (float)state.gameCanvasHeight);
    float CurrentViewportAspect =  (float)state.viewportWidth / (float)state.viewportHeight;
    
    if(CurrentViewportAspect > OriginalViewportAspect) {
        // Compensate for lack of Y
        state.compensationCalculation.x = 1;
        // kY = Xc / ((Xo/Yo) * Yc)
        state.compensationCalculation.y = (float)state.viewportWidth / (OriginalViewportAspect * (float)state.viewportHeight);
    } else if(CurrentViewportAspect < OriginalViewportAspect) {
        // Compensate for lack of X
        state.compensationCalculation.y = 1;
        // kX = ((Xo/Yo) * Yc) / Xc
        state.compensationCalculation.x = (OriginalViewportAspect * (float)state.viewportHeight) / (float)state.viewportWidth;
    } else {
        state.compensationCalculation.y = 1;
        state.compensationCalculation.x = 1;
    }
    
    state.NotifyChanged();
}