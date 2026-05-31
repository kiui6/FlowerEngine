#include "GlobalStateUpdate.h"

#include <glm/gtc/matrix_transform.hpp>

#include <Graphics/RenderEngine/RenderUtils.h>

void GlobalStateUpdate::Apply(GPUContext &gpu, RenderStateStore &store)
{
    if(!projectionMatrixDirty && !cameraPositionDirty) {
        return;
    }

    GlobalRenderState& state = store.GetMutable<GlobalRenderState>();

    if(projectionMatrixDirty) {
        state.canvasWidth = canvasWidth;
        state.canvasHeight = canvasHeight;

        state.projectionMatrix = glm::ortho(  0.0f, static_cast<float>(canvasWidth),
                                                static_cast<float>(canvasHeight), 0.0f,
                                                -65536.0f, 65536.0f);
    }

    if(cameraPositionDirty) {
        state.cameraPosition = cameraPosition;
    }


    GPUWorldData worldData{
        .projectionView = state.projectionMatrix,
        .position = state.cameraPosition
    };

    if(state.worldBuffer == nullptr) {
        state.worldBuffer = RenderUtils::CreateBuffer(gpu.device, SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ, &worldData, sizeof(worldData));
    } else {
        RenderUtils::UpdateBuffer(gpu.device, state.worldBuffer, &worldData, sizeof(worldData));
    }
    
    state.NotifyChanged();
}