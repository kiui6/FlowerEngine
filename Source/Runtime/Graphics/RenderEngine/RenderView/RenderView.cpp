#include "RenderView.h"

#include <Graphics/RenderEngine/RenderConstants.h>

void RenderView::SubmitJob(RenderJob *job)
{
    m_renderJobs.emplace(std::unique_ptr<RenderJob>(job));
}

RenderObject *RenderView::GetDynamicRenderObject(uint64_t id)
{
    auto it = m_dynamicRenderObjects.find(id);
    if(it != m_dynamicRenderObjects.end()) {
        return it->second.get();
    }

    const auto& pair = m_dynamicRenderObjects.emplace(id, std::make_unique<RenderObject>());

    return pair.first->second.get();
}

std::weak_ptr<RenderObject> RenderView::GetStaticRenderObject(uint64_t id)
{
    auto it = m_staticRenderObjects.find(id);
    if(it != m_staticRenderObjects.end()) {
        it->second.lastReferencedFrames = 0;
        return it->second.object;
    }
    StaticRenderObjectHandle handle;
    handle.object = std::make_shared<RenderObject>();
    handle.lastReferencedFrames = 0;

    const auto& pair = m_staticRenderObjects.emplace(id, handle);

    m_staticRenderObjectsDirty = true;

    return pair.first->second.object;
}

void RenderView::SubmitStateUpdate(RenderStateUpdate *stateUpdate)
{
    m_stateUpdates.emplace(std::unique_ptr<RenderStateUpdate>(stateUpdate));
}

void RenderView::Reset()
{
    m_staticRenderObjectsDirty = false;
    
    auto staticIt = m_staticRenderObjects.begin();
    while (staticIt != m_staticRenderObjects.end()) {
        if (staticIt->second.lastReferencedFrames > STATIC_OBJECTS_TIMEOUT_FRAMES) {
            staticIt = m_staticRenderObjects.erase(staticIt);
            m_staticRenderObjectsDirty = true;
        } else {
            staticIt->second.lastReferencedFrames++;
            ++staticIt;
        }
    }


    // Dynamic Render Objects should be recreated every frame.
    m_dynamicRenderObjects.clear();
    m_stateUpdates.clear();
    m_renderJobs.clear();
}
