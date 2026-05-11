#include "UpscaleRenderPass.h"

#include <Math/Mesh.h>
#include <Log/Log.h>

UpscaleRenderPass::UpscaleRenderPass(GPUContext &context)
{
    const uint32_t bufferSize = sizeof(Primitives::TRIANGLE) * sizeof(Vertex2D);

    SDL_GPUBufferCreateInfo bufInfo = {};
    bufInfo.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
    bufInfo.size = bufferSize;
    m_triangleVertBuffer = SDL_CreateGPUBuffer(context.device, &bufInfo);
    if(!m_triangleVertBuffer) {
        LOG(Fatal, LogUpscalePass, "Failed to create vertex buffer");
        return;
    }
    
    SDL_GPUTransferBufferCreateInfo tbInfo = {};
    tbInfo.size = bufferSize;
    tbInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    SDL_GPUTransferBuffer* tb = SDL_CreateGPUTransferBuffer(context.device, &tbInfo);

    if(!tb) {
        LOG(Fatal, LogUpscalePass, "Failed to create transfer buffer");
        return;
    }

    void* mapped = SDL_MapGPUTransferBuffer(context.device, tb, false);
    memcpy(mapped, &Primitives::TRIANGLE, bufferSize);
    SDL_UnmapGPUTransferBuffer(context.device, tb);

    SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(context.device);
    if(!cmd) {
        LOG(Fatal, LogUpscalePass, "Failed to acquire command buffer");
        return;
    }

    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmd);
    if(!copyPass) {
        LOG(Fatal, LogUpscalePass, "Failed to begin copy pass");
        return;
    }

    const SDL_GPUTransferBufferLocation tbBufferLocation = {
        .transfer_buffer = tb,
        .offset = 0
    };
    const SDL_GPUBufferRegion vBufferLocation = {
        .buffer = m_triangleVertBuffer,
        .offset = 0,
        .size = bufferSize
    };

    SDL_UploadToGPUBuffer(copyPass, &tbBufferLocation, &vBufferLocation, false);
    SDL_EndGPUCopyPass(copyPass);
    SDL_SubmitGPUCommandBuffer(cmd); 

    SDL_ReleaseGPUTransferBuffer(context.device, tb);
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