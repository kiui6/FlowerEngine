#include "UpscaleStateUpdate.h"

#include <glm/gtc/matrix_transform.hpp>

#include <Graphics/RenderEngine/RenderUtils.h>

void UpscaleStateUpdate::Apply(RenderStateUpdateContext& ctx)
{
    UpscaleRenderState& state = ctx.store.GetMutable<UpscaleRenderState>();

    float OriginalViewportAspect = ((float)gameCanvasWidth / (float)gameCanvasHeight);
    float CurrentViewportAspect =  (float)viewportWidth / (float)viewportHeight;
    
    if(CurrentViewportAspect > OriginalViewportAspect) {
        // Compensate for lack of Y
        state.compensationCalculation.x = 1;
        // kY = Xc / ((Xo/Yo) * Yc)
        state.compensationCalculation.y = (float)viewportWidth / (OriginalViewportAspect * (float)viewportHeight);
    } else if(CurrentViewportAspect < OriginalViewportAspect) {
        // Compensate for lack of X
        state.compensationCalculation.y = 1;
        // kX = ((Xo/Yo) * Yc) / Xc
        state.compensationCalculation.x = (OriginalViewportAspect * (float)viewportHeight) / (float)viewportWidth;
    } else {
        state.compensationCalculation.y = 1;
        state.compensationCalculation.x = 1;
    }
    
    state.NotifyChanged();
}