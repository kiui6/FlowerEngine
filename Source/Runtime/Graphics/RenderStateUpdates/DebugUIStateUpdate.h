#pragma once

#include <Graphics/RenderEngine/RenderView/RenderStateUpdate.h>
#include <Debug/DebugWindow.h>

#include <memory>

struct DebugUIStateUpdate : public RenderStateUpdate
{
    DebugUIStateUpdate(const std::shared_ptr<DebugWindow>& sharedWindow) : RenderStateUpdate({RenderPassType::DebugUI}, RenderStateUpdateType::DebugUI), window(sharedWindow) {}

    std::weak_ptr<DebugWindow> window;
};