#include "GlobalStateUpdate.h"

#include <glm/gtc/matrix_transform.hpp>

#include <Graphics/RenderEngine/RenderUtils.h>

void GlobalStateUpdate::Apply(RenderStateUpdateContext& ctx)
{
    if(!projectionMatrixDirty && !cameraPositionDirty) {
        return;
    }

    GlobalRenderState& state = ctx.store.GetMutable<GlobalRenderState>();

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

    
    if(state.worldBuffer[ctx.gpu.currentFrame] == nullptr) {
        state.worldBuffer[ctx.gpu.currentFrame] = RenderUtils::CreateBuffer(ctx.gpu.device, SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ, &worldData, sizeof(worldData));
        state.NotifyChanged();
        return;    
    }

    if(transferBuffer == nullptr) {
        transferBuffer = RenderUtils::CreateTransferBuffer(ctx.gpu.device, sizeof(worldData));
    }

    RenderUtils::UpdateBufferWithTransferBuffer(ctx.gpu.device, ctx.cmd, transferBuffer, state.worldBuffer[ctx.gpu.currentFrame], &worldData, sizeof(worldData));
    
    state.NotifyChanged();
}