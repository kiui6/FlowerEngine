#include "RenderUtils.h"

#include <Log/Log.h>

SDL_GPUBuffer *RenderUtils::CreateBuffer(SDL_GPUDevice *device, SDL_GPUBufferUsageFlags usage, void *data, uint32_t size)
{
    SDL_GPUBufferCreateInfo bufInfo = {};
    bufInfo.usage = usage;
    bufInfo.size = size;
    SDL_GPUBuffer* buffer = SDL_CreateGPUBuffer(device, &bufInfo);
    if(!buffer) {
        LOG(Assert, LogCreateBuffer, "Failed to create buffer");
        return nullptr;
    }

    if(!data) { 
        return buffer;
    }
    
    SDL_GPUTransferBufferCreateInfo tbInfo = {};
    tbInfo.size = size;
    tbInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    SDL_GPUTransferBuffer* tb = SDL_CreateGPUTransferBuffer(device, &tbInfo);

    if(!tb) {
        LOG(Assert, LogCreateBuffer, "Failed to create transfer buffer");
        return nullptr;
    }

    void* mapped = SDL_MapGPUTransferBuffer(device, tb, false);
    memcpy(mapped, data, size);
    SDL_UnmapGPUTransferBuffer(device, tb);

    SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(device);
    if(!cmd) {
        LOG(Assert, LogCreateBuffer, "Failed to acquire command buffer");
        return nullptr;
    }

    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmd);
    if(!copyPass) {
        LOG(Assert, LogCreateBuffer, "Failed to begin copy pass");
        return nullptr;
    }

    const SDL_GPUTransferBufferLocation tbBufferLocation = {
        .transfer_buffer = tb,
        .offset = 0
    };
    const SDL_GPUBufferRegion vBufferLocation = {
        .buffer = buffer,
        .offset = 0,
        .size = size
    };

    SDL_UploadToGPUBuffer(copyPass, &tbBufferLocation, &vBufferLocation, false);
    SDL_EndGPUCopyPass(copyPass);
    SDL_SubmitGPUCommandBuffer(cmd); 

    SDL_ReleaseGPUTransferBuffer(device, tb);

    return buffer;
}

bool RenderUtils::UpdateBuffer(SDL_GPUDevice *device, SDL_GPUBuffer* buffer, void *data, uint32_t size)
{
    SDL_GPUTransferBufferCreateInfo tbInfo = {};
    tbInfo.size = size;
    tbInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
    SDL_GPUTransferBuffer* tb = SDL_CreateGPUTransferBuffer(device, &tbInfo);

    if(!tb) {
        LOG(Assert, LogUpdateBuffer, "Failed to create transfer buffer");
        return false;
    }

    void* mapped = SDL_MapGPUTransferBuffer(device, tb, false);
    memcpy(mapped, data, size);
    SDL_UnmapGPUTransferBuffer(device, tb);

    SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(device);
    if(!cmd) {
        LOG(Assert, LogUpdateBuffer, "Failed to acquire command buffer");
        return false;
    }

    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(cmd);
    if(!copyPass) {
        LOG(Assert, LogUpdateBuffer, "Failed to begin copy pass");
        return false;
    }

    const SDL_GPUTransferBufferLocation tbBufferLocation = {
        .transfer_buffer = tb,
        .offset = 0
    };
    const SDL_GPUBufferRegion vBufferLocation = {
        .buffer = buffer,
        .offset = 0,
        .size = size
    };

    SDL_UploadToGPUBuffer(copyPass, &tbBufferLocation, &vBufferLocation, false);
    SDL_EndGPUCopyPass(copyPass);
    SDL_SubmitGPUCommandBuffer(cmd); 

    SDL_ReleaseGPUTransferBuffer(device, tb);

    return true;
}

SDL_GPUShader *RenderUtils::CreateShader(SDL_GPUDevice *device, SDL_GPUShaderStage stage, const unsigned char* data, uint32_t size, uint32_t numSamplers, uint32_t numStorageTextures, uint32_t numStorageBuffers, uint32_t numUniformBuffers)
{
    SDL_GPUShaderCreateInfo createInfo{
        .code                   = reinterpret_cast<const Uint8*>(data),
        .code_size              = size,
        .entrypoint             = "main",
        .format                 = SDL_GPU_SHADERFORMAT_SPIRV,
        .stage                  = stage,
        .num_samplers           = numSamplers,
        .num_storage_buffers    = numStorageBuffers,
        .num_storage_textures   = numStorageTextures,
        .num_uniform_buffers    = numUniformBuffers
    };
    return SDL_CreateGPUShader(device, &createInfo);
}
