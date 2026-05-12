#include "ResourceCompiler.h"

#include <Log/Log.h>

int RenderResourceCompiler::CompileTexture2D(Texture2DResource &tex2d)
{
    auto it = m_resources.texture2D.find(tex2d.id);
    if(it != m_resources.texture2D.end()) {
        // No need to update anything if resource is not reported as dirty
        if(!tex2d.isDirty) {
            return 0;
        }

        const std::shared_ptr<CompiledTexture2DResource>& compiled = it->second;

        if(tex2d.width != compiled->width || tex2d.height != compiled->height) {
            LOG(Assert, LogRenderResourceCompiler, "Updated texture has different dimensions. That requires texture recreation, which is not currently supported.");
            return 1;
        }

        // TODO: We can't change format after creation, so check for format change

        SDL_GPUTransferBufferCreateInfo tbInfo = {};
        tbInfo.size = tex2d.dataSize;
        tbInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
        SDL_GPUTransferBuffer* tb = SDL_CreateGPUTransferBuffer(m_ctx.device, &tbInfo);

        void* mapped = SDL_MapGPUTransferBuffer(m_ctx.device, tb, false);
        memcpy(mapped, tex2d.data, tbInfo.size);
        SDL_UnmapGPUTransferBuffer(m_ctx.device, tb);

        SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(m_ctx.device);

        SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmd);

        SDL_GPUTextureTransferInfo transfer = {tb, 0, compiled->width, compiled->height};
        SDL_GPUTextureRegion region = { compiled->texture, 0, 0, 0, 0, 0, compiled->width, compiled->height, 1 };

        SDL_UploadToGPUTexture(copyPass, &transfer, &region, false);

        SDL_EndGPUCopyPass(copyPass);

        SDL_SubmitGPUCommandBuffer(cmd);

        SDL_ReleaseGPUTransferBuffer(m_ctx.device, tb);
        
        tex2d.m_compiledResource = compiled;
        tex2d.isDirty = false;
    }

    std::shared_ptr<CompiledTexture2DResource> compiled = std::make_shared<CompiledTexture2DResource>();
    compiled->width = tex2d.width;
    compiled->height = tex2d.height;
    
    // TODO: Add all formats
    switch(tex2d.components) {
        case Texture2DComponents::A8_UNORM:
            compiled->format = SDL_GPU_TEXTUREFORMAT_A8_UNORM;
            break;
        case Texture2DComponents::R8G8B8A8_UNORM:
        compiled->format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
            break;
        case Texture2DComponents::R8G8_UINT:
            compiled->format = SDL_GPU_TEXTUREFORMAT_R8G8_UINT;
            break;
        case Texture2DComponents::R16G16_UINT:
            compiled->format = SDL_GPU_TEXTUREFORMAT_R16G16_UINT;
            break;
        case Texture2DComponents::R32G32_UINT:
            compiled->format = SDL_GPU_TEXTUREFORMAT_R32G32_UINT;
            break;
        default:
            LOG(Assert, LogRenderResourceCompiler, "Unsupported texture format!");
            return 1;
    }

    SDL_GPUTextureCreateInfo texInfo = {};
    texInfo.type = SDL_GPU_TEXTURETYPE_2D;
    texInfo.format = compiled->format;
    texInfo.width = compiled->width;
    texInfo.height = compiled->height;
    texInfo.layer_count_or_depth = 1;
    texInfo.num_levels = 1;
    texInfo.usage = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER;
    compiled->texture = SDL_CreateGPUTexture(m_ctx.device, &texInfo);
    if(!compiled->texture) {
        LOG(Assert, LogRenderResourceCompiler, "Failed to create GPU texture!");
        return 1;
    }

    SDL_GPUTransferBufferCreateInfo tbInfo = {};
    tbInfo.size = tex2d.dataSize;
    tbInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    SDL_GPUTransferBuffer* tb = SDL_CreateGPUTransferBuffer(m_ctx.device, &tbInfo);

    void* mapped = SDL_MapGPUTransferBuffer(m_ctx.device, tb, false);
    memcpy(mapped, tex2d.data, tbInfo.size);
    SDL_UnmapGPUTransferBuffer(m_ctx.device, tb);

    SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(m_ctx.device);

    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmd);

    SDL_GPUTextureTransferInfo transfer = {tb, 0, compiled->width, compiled->height};
    SDL_GPUTextureRegion region = { compiled->texture, 0, 0, 0, 0, 0, compiled->width, compiled->height, 1 };

    SDL_UploadToGPUTexture(copyPass, &transfer, &region, false);

    SDL_EndGPUCopyPass(copyPass);

    SDL_SubmitGPUCommandBuffer(cmd);

    SDL_ReleaseGPUTransferBuffer(m_ctx.device, tb);

    auto emplacePair = m_resources.texture2D.emplace(tex2d.id, compiled);

    tex2d.m_compiledResource = emplacePair.first->second;
    tex2d.isDirty = false;

    return 0;
}