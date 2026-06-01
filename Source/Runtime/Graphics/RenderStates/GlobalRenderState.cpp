#include "GlobalRenderState.h"

#include <Graphics/RenderEngine/RenderUtils.h>
#include <Config/Config.h>

#include <glm/gtc/matrix_transform.hpp>

GlobalRenderState::GlobalRenderState(GPUContext &gpu)
    : RenderState(gpu)
{
    Config* config = GetService<Config>();

    canvasWidth = config->GetNamespace("Game").GetInt("Render.DefaultCanvasWidth", 640);
    canvasHeight = config->GetNamespace("Game").GetInt("Render.DefaultCanvasHeight", 360);

    projectionMatrix = glm::ortho(  0.0f, static_cast<float>(canvasWidth),
                                                static_cast<float>(canvasHeight), 0.0f,
                                                -65536.0f, 65536.0f);

    GPUWorldData worldData{
        .projectionView = projectionMatrix,
        .position = cameraPosition
    };

    for(int i = 0; i < worldBuffer.size(); ++i) {
        worldBuffer[i] = RenderUtils::CreateBuffer(gpu.device, SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ, &worldData, sizeof(worldData));
    }
}

GlobalRenderState::~GlobalRenderState()
{
    for(int i = 0; i < worldBuffer.size(); ++i) {
        SDL_ReleaseGPUBuffer(m_gpu.device, worldBuffer[i]);
    }
}
