#pragma once

#include <Graphics/RenderEngine/RenderState/RenderStateUpdate.h>
#include <Debug/DebugWindow.h>

#include <memory>

struct DebugUIStateUpdate : public RenderStateUpdate
{
    DebugUIStateUpdate(const std::shared_ptr<DebugWindow>& win) : window(win) {}

    std::weak_ptr<DebugWindow> window;

    virtual void Apply(RenderStateStore& store) override;
};