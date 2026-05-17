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

#include <SDL3/SDL_gpu.h>

#include "GPUContext.h"
#include "ResourceCompiler.h"
#include "RenderUtils.h"
#include "RenderConstants.h"

#include "RenderState/RenderStateStore.h"

class RenderEngine
{
    std::array<RenderView, FRAMES_IN_FLIGHT> m_renderViews;

    SDL_GPUCommandBuffer *m_renderBuffer, *m_onDemandBuffer;

    std::array<std::unique_ptr<RenderPass>, (uint32_t)RenderPassType::MAX> m_renderPasses;

    GPUContext m_ctx;

    RenderStateStore m_stateStore;

    CompiledRenderResources m_compiledRes;
public:
    RenderEngine();

    RenderView& GetFrameRenderView() { return m_renderViews[(m_ctx.currentFrame + 1) % m_renderViews.size()];}

    void Initialize(SDL_Window* window);

    void Render(float deltaTime, RenderView& renderView);

protected:
};