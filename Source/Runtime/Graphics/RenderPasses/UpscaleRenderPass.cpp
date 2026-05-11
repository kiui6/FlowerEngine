#include "UpscaleRenderPass.h"

void UpscaleRenderPass::Render(FrameContext& ctx)
{
    SDL_PushGPUDebugGroup(ctx.cmd, "Upscale");

        SDL_GPUColorTargetInfo colorTarget = {
            .texture     = ctx.swapchainTexture,
            .mip_level   = 0,
            .layer_or_depth_plane = 0,
            .clear_color = (SDL_FColor){ 0.88f, 0.722f, 0.23f, 1.0f },
            .load_op     = SDL_GPU_LOADOP_CLEAR,
            .store_op    = SDL_GPU_STOREOP_STORE,
        };

    SDL_GPURenderPass* pass = SDL_BeginGPURenderPass(ctx.cmd, &colorTarget, 1, NULL);
    SDL_EndGPURenderPass(pass);

    SDL_PopGPUDebugGroup(ctx.cmd);
}