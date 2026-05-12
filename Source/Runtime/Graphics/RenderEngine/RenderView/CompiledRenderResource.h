#pragma once

#include "Texture2DComponents.h"

#include <SDL3/SDL_gpu.h>

struct CompiledRenderResource {};

struct CompiledTexture2DResource : public CompiledRenderResource
{
    uint32_t width, height;
    SDL_GPUTextureFormat format;
    SDL_GPUTexture* texture;
};