#include "RenderView.h"

RenderObject* RenderView::GetDynamicRenderObject(uint64_t id)
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
        it->second.referenced = true;
        return it->second.object;
    }
    StaticRenderObjectHandle handle;
    handle.object = std::make_shared<RenderObject>();
    handle.referenced = true;

    const auto& pair = m_staticRenderObjects.emplace(id, handle);

    m_staticRenderObjectsDirty = true;

    return pair.first->second.object;
}

void RenderView::SubmitStateUpdate(RenderStateUpdate *stateUpdate)
{
    m_stateUpdates.emplace_back(std::unique_ptr<RenderStateUpdate>(stateUpdate));
}

void RenderView::Reset()
{
    auto staticIt = m_staticRenderObjects.begin();
    while (staticIt != m_staticRenderObjects.end()) {
        if (!staticIt->second.referenced) {
            staticIt = m_staticRenderObjects.erase(staticIt);
        } else {
            staticIt->second.referenced = false;
            ++staticIt;
        }
    }

    m_staticRenderObjectsDirty = false;

    // Dynamic Render Objects should be recreated every frame.
    m_dynamicRenderObjects.clear();
    m_stateUpdates.clear();
}
