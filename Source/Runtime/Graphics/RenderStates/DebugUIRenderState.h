#pragma once

#include <Graphics/RenderEngine/RenderState/RenderState.h>

class DebugWindow;

struct DebugUIRenderState : public RenderState, public StaticallyTyped<MakeID32("DBUI")>
{
    DebugUIRenderState(GPUContext& gpu) : RenderState(gpu) {}

    std::weak_ptr<DebugWindow> window;
};