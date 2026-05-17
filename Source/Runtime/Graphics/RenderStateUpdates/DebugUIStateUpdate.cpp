#include "DebugUIStateUpdate.h"

#include <Graphics/RenderStates/DebugUIRenderState.h>

void DebugUIStateUpdate::Apply(GPUContext& gpu, RenderStateStore &store)
{
    DebugUIRenderState& state = store.GetMutable<DebugUIRenderState>();

    state.window = window;
    
    state.NotifyChanged();
}