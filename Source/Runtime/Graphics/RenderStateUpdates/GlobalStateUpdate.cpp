#include "GlobalStateUpdate.h"

#include <glm/gtc/matrix_transform.hpp>

#include <Graphics/RenderEngine/RenderUtils.h>

void GlobalStateUpdate::Apply(RenderStateUpdateContext& ctx)
{
    GlobalRenderState& state = ctx.store.GetMutable<GlobalRenderState>();

    if(canvasDirty) {
        state.canvasWidth = canvasWidth;
        state.canvasHeight = canvasHeight;
    }
    
    state.NotifyChanged();
}