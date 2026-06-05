#pragma once

#include <Graphics/RenderEngine/BufferAttachmentUpdateHandler.h>
#include <Graphics/RenderEngine/RenderConstants.h>

#include <Graphics/RenderStates/GlobalRenderState.h>

class SDL_GPUTransferBuffer;

class WorldBufferAttachmentUpdateHandler : public IBufferAttachmentUpdateHandler {
    const GlobalRenderState& globalState;

    std::array<SDL_GPUTransferBuffer*, FRAMES_IN_FLIGHT> m_transferBuffers; 
public:
    WorldBufferAttachmentUpdateHandler(GPUContext& gpu, const RenderStateStore& stateStore);
    ~WorldBufferAttachmentUpdateHandler() = default;
    virtual void Update(FrameContext& ctx, const RenderView& view, SDL_GPUCopyPass* copyPass) override;
};