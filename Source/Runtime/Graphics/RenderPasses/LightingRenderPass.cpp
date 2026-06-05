#include "LightingRenderPass.h"

LightingRenderPass::LightingRenderPass(GPUContext &context)
{
}

LightingRenderPass::~LightingRenderPass()
{
}

void LightingRenderPass::RenderFrame(FrameContext &ctx)
{
    BeginGPULabel(ctx.cmd, "Lighting");

    EndGPULabel(ctx.cmd);
}