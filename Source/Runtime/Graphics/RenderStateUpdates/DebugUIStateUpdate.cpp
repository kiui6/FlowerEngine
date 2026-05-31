#include "DebugUIStateUpdate.h"

#include <Graphics/RenderStates/DebugUIRenderState.h>

void DebugUIStateUpdate::Apply(RenderStateUpdateContext& ctx)
{
    DebugUIRenderState& state = ctx.store.GetMutable<DebugUIRenderState>();

    state.window = window;
    
    state.NotifyChanged();
}