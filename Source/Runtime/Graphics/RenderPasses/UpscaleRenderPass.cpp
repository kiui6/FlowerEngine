#include "UpscaleRenderPass.h"

UpscaleRenderPass::UpscaleRenderPass(GPUContext &context)
{
}

UpscaleRenderPass::~UpscaleRenderPass()
{
}

void UpscaleRenderPass::Render(FrameContext &ctx)
{
    BeginGPULabel(ctx.cmd, "Upscale");

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

    EndGPULabel(ctx.cmd);
}