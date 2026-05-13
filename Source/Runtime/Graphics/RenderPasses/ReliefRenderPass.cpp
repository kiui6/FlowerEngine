#include "ReliefRenderPass.h"

#include <Config/Config.h>

ReliefRenderPass::ReliefRenderPass(GPUContext &context)
    :m_gpu(context)
{
    Config* config = GetService<Config>();

    SDL_GPUTextureCreateInfo texInfo = {};
    texInfo.type = SDL_GPU_TEXTURETYPE_2D;
    texInfo.format = SDL_GPU_TEXTUREFORMAT_R32G32_UINT;
    texInfo.width = config->GetNamespace("Game").GetInt("Render.PixelPerfect.CanvasWidth", 320);
    texInfo.height = config->GetNamespace("Game").GetInt("Render.PixelPerfect.CanvasWidth", 180);
    texInfo.layer_count_or_depth = 1;
    texInfo.num_levels = 1;
    texInfo.usage = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER;
    m_relief = SDL_CreateGPUTexture(m_gpu.device, &texInfo);
}

ReliefRenderPass::~ReliefRenderPass()
{
    SDL_ReleaseGPUTexture(m_gpu.device, m_relief);
}

void ReliefRenderPass::Render(FrameContext &ctx)
{
    ctx.attachments[(uint8_t)RenderAttachment::Relief] = m_relief;

    BeginGPULabel(ctx.cmd, "Relief");

    EndGPULabel(ctx.cmd);
}