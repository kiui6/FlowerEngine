#pragma once

#include <SDL3/SDL_gpu.h>

#include "Attachment.h"

#include <array>

class CompiledRenderResources;
class RenderPass;

struct FrameContext {
    SDL_GPUCommandBuffer* cmd;
    float deltaTime = 0.0f;
    SDL_GPUTexture* swapchainTexture;
    CompiledRenderResources* resources;
    RenderPass* previousPass;

    std::array<SDL_GPUTexture*, (uint8_t)RenderAttachment::MAX> attachments;
};