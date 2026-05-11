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
    static bool UpdateBuffer(SDL_GPUDevice* device, SDL_GPUBuffer* buffer, void* data, uint32_t size);
};