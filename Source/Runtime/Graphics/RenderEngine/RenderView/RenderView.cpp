#include "RenderView.h"

#include <Graphics/RenderEngine/RenderConstants.h>

#include <Debug/Tracer/Tracer.h>

void RenderView::SubmitJob(RenderJob *job)
{
    m_renderJobs.emplace(std::unique_ptr<RenderJob>(job));
}

RenderObject& RenderView::AddDynamicRenderObject(uint64_t id)
{
    return m_dynamicRenderObjects.emplace_back(RenderObject{});
}

RenderObject* RenderView::GetStaticRenderObject(uint64_t id)
{
    auto it = m_staticRenderObjects.find(id);
    if(it != m_staticRenderObjects.end()) {
        it->second.lastReferencedFramesAgo = 0;
        return &it->second.object;
    }
    
    return nullptr;
}

bool RenderView::HasStaticRenderObject(uint64_t id)
{
    return m_staticRenderObjects.find(id) != m_staticRenderObjects.end();
}

RenderObject& RenderView::AddStaticRenderObject(uint64_t id)
{
    RenderObjectHandle handle;
    handle.object = RenderObject{};
    handle.lastReferencedFramesAgo = 0;

    const auto& pair = m_staticRenderObjects.emplace(id, handle);

    m_staticRenderObjectsDirty = true;

    return pair.first->second.object;
}

void RenderView::Reset()
{
    PUSH_TRACE_SCOPE("RenderView::Reset()");
    
    size_t staticRObjectsCountBeforeReset = m_staticRenderObjects.size();

    auto staticRObjectsIt = m_staticRenderObjects.begin();
    while (staticRObjectsIt != m_staticRenderObjects.end()) {
        if (staticRObjectsIt->second.lastReferencedFramesAgo > STATIC_OBJECTS_TIMEOUT_FRAMES) {
            staticRObjectsIt = m_staticRenderObjects.erase(staticRObjectsIt);
        } else {
            staticRObjectsIt->second.lastReferencedFramesAgo++;
            ++staticRObjectsIt;
        }
    }

    m_staticRenderObjectsDirty = staticRObjectsCountBeforeReset != m_staticRenderObjects.size();

    // Dynamic Render Objects should be recreated every frame.
    m_dynamicRenderObjects.clear();
    m_renderJobs.clear();

    POP_TRACE_SCOPE();
}
