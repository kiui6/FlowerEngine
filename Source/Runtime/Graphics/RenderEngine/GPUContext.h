#pragma once

#include <SDL3/SDL_gpu.h>

struct GPUContext {
    SDL_Window* window;
    SDL_GPUDevice* device;
    SDL_GPUSwapchainComposition swapchainComposition;
    SDL_GPUTextureFormat swapchainFormat;
    SDL_GPUPresentMode presentMode;
    uint8_t currentFrame = 0;
    bool vsync;
};