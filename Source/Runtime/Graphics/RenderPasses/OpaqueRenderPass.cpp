#include "OpaqueRenderPass.h"

#include <SDL3/SDL_gpu.h>

void OpaqueRenderPass::Render(FrameContext& ctx)
{
    SDL_PushGPUDebugGroup(ctx.cmd, "Opaque");

    SDL_PopGPUDebugGroup(ctx.cmd);
}