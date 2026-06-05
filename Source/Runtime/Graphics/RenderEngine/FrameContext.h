#pragma once

#include <SDL3/SDL_gpu.h>

#include "ImageAttachment.h"
#include "BufferAttachment.h"

#include <Utility/EnumClassArray.h>

class CompiledRenderResources;
class RenderPass;

struct FrameContext {
    SDL_GPUCommandBuffer* cmd;
    float deltaTime = 0.0f;
    uint8_t frameIndex = 0;
    SDL_GPUTexture* swapchainTexture;
    uint32_t swapchainWidth, swapchainHeight;
    CompiledRenderResources* resources;
    RenderPass* previousPass;

    EnumClassArray<SDL_GPUTexture*, ImageRenderAttachment> imageAttachments{};
    EnumClassArray<SDL_GPUBuffer*, BufferRenderAttachment> bufferAttachments{};
};