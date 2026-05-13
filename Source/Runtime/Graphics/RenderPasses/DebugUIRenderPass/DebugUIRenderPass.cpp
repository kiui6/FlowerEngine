#include "DebugUIRenderPass.h"

#include <Graphics/RenderEngine/RenderView/RenderStateUpdate.h>

DebugUIRenderPass::DebugUIRenderPass(GPUContext &context)
    : m_gpu(context)
{
    
}

DebugUIRenderPass::~DebugUIRenderPass()
{
}

void DebugUIRenderPass::UpdateState(RenderStateUpdate *updateObj)
{
#ifdef HAS_IMGUI
    if(updateObj->GetUpdateType() == RenderStateUpdateType::DebugUI) {
        m_window = static_cast<DebugUIStateUpdate*>(updateObj)->window.lock();
    }
#endif
}

void DebugUIRenderPass::Render(FrameContext &ctx)
{
#ifdef HAS_IMGUI

#endif
}
