#pragma once

#include "RenderView/RenderView.h"
#include "RenderPass/RenderPass.h"
#include "RenderView/CompiledRenderResource.h"
#include "RenderView/RenderResourceContainer.h"
#include "OnDemand/OnDemandRenderTask.h"

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

class RenderEngine
{
    std::array<RenderView, FRAMES_IN_FLIGHT> m_renderViews;
    uint8_t m_currentFrame = 0;

    SDL_GPUCommandBuffer *m_renderBuffer, *m_onDemandBuffer;

    std::array<std::unique_ptr<RenderPass>, (uint32_t)RenderPassType::MAX> m_renderPasses;

    std::set<std::unique_ptr<OnDemandRenderTask>> m_onDemandTasks;
    std::mutex m_onDemandMtx;

    GPUContext m_ctx;

    CompiledRenderResources m_compiledRes;
public:
    RenderView& GetFrameRenderView() { return m_renderViews[(m_currentFrame + 1) % m_renderViews.size()];}

    void Initialize(SDL_Window* window);

    void Render(float deltaTime, RenderView& renderView);

    void SubmitOnDemandTask(OnDemandRenderTask* task){std::unique_lock lock(m_onDemandMtx); m_onDemandTasks.insert(std::unique_ptr<OnDemandRenderTask>(task));}
protected:
};