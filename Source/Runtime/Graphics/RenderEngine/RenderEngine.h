#pragma once

#include "RenderView/RenderView.h"
#include "RenderPass/RenderPass.h"
#include "RenderView/CompiledRenderResource.h"
#include "RenderView/RenderResourceContainer.h"
#include "RenderJob/RenderJob.h"

#include <array>
#include <vector>
#include <unordered_map>
#include <map>
#include <array>
#include <set>
#include <mutex>

#include <Utility/Meta/AlwaysFalse.h>

#include <SDL3/SDL_gpu.h>

#include "GPUContext.h"
#include "ResourceCompiler.h"
#include "RenderUtils.h"
#include "RenderConstants.h"
#include "BufferAttachmentUpdateHandler.h"

#include "RenderPass/RenderPassTypes.h"
#include "RenderState/RenderStateStore.h"

class RenderEngine
{
    std::array<RenderView, FRAMES_IN_FLIGHT> m_renderViews;
    std::array<FrameContext, FRAMES_IN_FLIGHT> m_frameContexts;

    SDL_GPUCommandBuffer *m_renderBuffer, *m_computeBuffer;

    EnumClassArray<std::unique_ptr<IBufferAttachmentUpdateHandler>, BufferRenderAttachment> m_frameBufferAttachmentUpdateHandlers;
    EnumClassArray<std::unique_ptr<RenderPass>, RenderPassType> m_renderPasses;

    GPUContext m_ctx;

    RenderStateStore m_stateStore;

    CompiledRenderResources m_compiledRes;
public:
    RenderEngine();

    RenderView& GetFrameRenderView() { return m_renderViews[(m_ctx.currentFrame + 1) % m_renderViews.size()];}

    void Initialize(SDL_Window* window);

    void Render(float deltaTime, RenderView& renderView);

protected:
    template<typename T>
    inline std::unique_ptr<T> MakeRenderModule();
};

template<typename T>
inline std::unique_ptr<T> RenderEngine::MakeRenderModule()
{
    if constexpr (std::is_constructible_v<T, GPUContext&, RenderStateStore&>) {
        return std::make_unique<T>(m_ctx, m_stateStore);
    } else if constexpr (std::is_constructible_v<T, GPUContext&>) {
        return std::make_unique<T>(m_ctx);
    } else {
        static_assert(AlwaysFalse<T>::value, "T has no suitable constructor");
    }
}