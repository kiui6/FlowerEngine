#pragma once

#include "FrameContext.h"
#include "RenderView/RenderView.h"

#include "GPUContext.h"

class IBufferAttachmentUpdateHandler {
protected:
    GPUContext& m_gpu;
public:
    IBufferAttachmentUpdateHandler(GPUContext& gpu) : m_gpu(gpu) {}
    virtual ~IBufferAttachmentUpdateHandler() = default;

    virtual void Update(FrameContext& ctx, const RenderView& view, SDL_GPUCopyPass* copyPass) = 0;
};