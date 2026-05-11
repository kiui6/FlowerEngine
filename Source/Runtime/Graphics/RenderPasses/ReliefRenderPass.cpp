#include "ReliefRenderPass.h"

void ReliefRenderPass::Render(FrameContext& ctx)
{
    SDL_PushGPUDebugGroup(ctx.cmd, "Relief");

    SDL_PopGPUDebugGroup(ctx.cmd);
}