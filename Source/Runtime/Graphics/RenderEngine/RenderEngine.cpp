#include "RenderEngine.h"

#include <Log/Log.h>

void RenderEngine::Initialize()
{
    // TODO: m_renderPasses[(uint32_t)RenderPassType::Opaque] = std::make_unique<OpaqueRenderPass>();
}

void RenderEngine::Render(float deltaTime, RenderView &renderView)
{
    std::unique_lock lock(m_onDemandMtx); 

    // Perform State Changes

    // Execute OnDemand Tasks

    // Render Frame
    for(size_t passIdx = 0; passIdx < m_renderPasses.size(); passIdx++) {
        // TODO: m_renderPasses[passIdx]->Render();
    }

    // Cleanup render view after frame is rendered
    renderView.Reset();
}
