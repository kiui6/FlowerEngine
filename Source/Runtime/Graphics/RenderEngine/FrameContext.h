#pragma once

#include <SDL3/SDL_gpu.h>

#include "Attachment.h"

#include <array>

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

    std::array<SDL_GPUTexture*, (uint8_t)RenderAttachment::MAX> attachments;
};