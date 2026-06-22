#include "WorldBufferAttachmentUpdateHandler.h"

#include <glm/gtc/matrix_transform.hpp>

#include <Graphics/BufferAttachments/WorldBufferAttachment.h>
#include <Graphics/RenderEngine/RenderUtils.h>

#include <Log/Log.h>

WorldBufferAttachmentUpdateHandler::WorldBufferAttachmentUpdateHandler(GPUContext &gpu, const RenderStateStore& stateStore)
    : IBufferAttachmentUpdateHandler(gpu), globalState(stateStore.Get<GlobalRenderState>())
{
    for(auto& buffer : m_transferBuffers) {
        buffer = RenderUtils::CreateTransferBuffer(gpu.device, sizeof(GPUWorldBufferData));
    }
}

void WorldBufferAttachmentUpdateHandler::Update(FrameContext &ctx, const RenderView &view, SDL_GPUCopyPass* copyPass)
{
    SDL_GPUBuffer*& buffer = ctx.bufferAttachments[BufferRenderAttachment::WorldData];

    GPUWorldBufferData gpuData;
    gpuData.projectionView = glm::ortho(  -static_cast<float>(globalState.canvasWidth) / 2, static_cast<float>(globalState.canvasWidth) / 2,
                                                static_cast<float>(globalState.canvasHeight) / 2, -static_cast<float>(globalState.canvasHeight) / 2,
                                                -65536.0f, 65536.0f);
    gpuData.position = view.GetCameraPosition();

    if(buffer == nullptr) {
        buffer = RenderUtils::CreateBuffer(m_gpu.device, SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ, &gpuData, sizeof(gpuData));
    }

    if(!RenderUtils::UpdateBufferWithTransferBufferAndPass(m_gpu.device, ctx.cmd, copyPass, m_transferBuffers[ctx.frameIndex], buffer, &gpuData, sizeof(gpuData))) {
        LOG(Error, LogWorldBufferAttachmentUpdateHandler, "Failed updating world buffer attachment");
        return;
    }
}