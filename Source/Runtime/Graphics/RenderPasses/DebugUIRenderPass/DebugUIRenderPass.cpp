#include "DebugUIRenderPass.h"

DebugUIRenderPass::DebugUIRenderPass(GPUContext &context, RenderStateStore& stateStore)
    : m_gpu(context), m_state(stateStore.Get<DebugUIRenderState>())
{
    
}

DebugUIRenderPass::~DebugUIRenderPass()
{
}

void DebugUIRenderPass::Render(FrameContext &ctx)
{
#ifdef HAS_IMGUI

#endif
}
