#include "DebugUIStateUpdate.h"

#include <Graphics/RenderStates/DebugUIRenderState.h>

void DebugUIStateUpdate::Apply(RenderStateStore &store)
{
    store.GetMutable<DebugUIRenderState>().window = window;
}