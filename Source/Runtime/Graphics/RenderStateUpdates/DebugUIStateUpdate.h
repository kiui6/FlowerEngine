#pragma once

#include <Graphics/RenderEngine/RenderState/RenderStateUpdate.h>
#include <Debug/DebugWindow.h>

#include <memory>

struct DebugUIStateUpdate : public RenderStateUpdate, public StaticallyTyped<MakeID32("DBUI")>
{
    std::weak_ptr<DebugWindow> window;

    virtual void Apply(RenderStateUpdateContext& ctx) override;
};