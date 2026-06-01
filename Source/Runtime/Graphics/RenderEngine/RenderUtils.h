#pragma once

#ifdef NDEBUG
#   define BeginGPULabel(cmd, label)
#   define EndGPULabel(cmd)
#else
#   define BeginGPULabel(cmd, label) SDL_PushGPUDebugGroup( cmd , label )
#   define EndGPULabel(cmd) SDL_PopGPUDebugGroup( cmd )
#endif

#include <SDL3/SDL_gpu.h>

struct RenderUtils {
    static SDL_GPUBuffer* CreateBuffer(SDL_GPUDevice* device, SDL_GPUBufferUsageFlags usage, void* data, uint32_t size);
    static SDL_GPUTransferBuffer* CreateTransferBuffer(SDL_GPUDevice* device, uint32_t size);
    static bool UpdateBuffer(SDL_GPUDevice* device, SDL_GPUBuffer* buffer, void* data, uint32_t size);
    static bool UpdateBufferWithTransferBuffer(SDL_GPUDevice *device, SDL_GPUCommandBuffer* cmd, SDL_GPUTransferBuffer* transfer, SDL_GPUBuffer* buffer, void* data, uint32_t size);
    static bool UpdateBufferWithTransferBufferAndPass(SDL_GPUDevice *device, SDL_GPUCommandBuffer* cmd, SDL_GPUCopyPass* pass, SDL_GPUTransferBuffer* transfer, SDL_GPUBuffer* buffer, void* data, uint32_t size);

    static SDL_GPUShader* CreateShader( SDL_GPUDevice* device, 
                                        SDL_GPUShaderStage stage,
                                        const unsigned char*  data, uint32_t size, 
                                        uint32_t numSamplers,
                                        uint32_t numStorageTextures,
                                        uint32_t numStorageBuffers,
                                        uint32_t numUniformBuffers);

    // TODO
    static SDL_GPUGraphicsPipeline* CreatePipeline(SDL_GPUDevice* device);
};