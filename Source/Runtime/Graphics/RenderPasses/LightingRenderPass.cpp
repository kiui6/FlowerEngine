#include "LightingRenderPass.h"

void LightingRenderPass::Render(FrameContext& ctx)
{
    SDL_PushGPUDebugGroup(ctx.cmd, "Lighting");

    SDL_PopGPUDebugGroup(ctx.cmd);
}