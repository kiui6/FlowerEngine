#pragma once

#include <Graphics/RenderEngine/RenderState/RenderState.h>

class DebugWindow;

struct DebugUIRenderState : public RenderState
{
    static ID32 StaticType() {return MakeID32("DBUI");} 

    std::weak_ptr<DebugWindow> window;
};