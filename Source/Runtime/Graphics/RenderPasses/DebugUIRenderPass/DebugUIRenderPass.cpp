#include "DebugUIRenderPass.h"

DebugUIRenderPass::DebugUIRenderPass(GPUContext &context)
    : m_gpu(context)
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
