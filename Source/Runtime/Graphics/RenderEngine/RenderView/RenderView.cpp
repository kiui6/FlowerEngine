#include "RenderView.h"

void RenderView::AddDynamicRenderObject(uint64_t id, std::unique_ptr<RenderObject> pRendObj)
{
    if(pRendObj && pRendObj->GetElementsCount())
        m_dynamicRenderObjects.emplace(id, std::move(pRendObj));
}

void RenderView::SubmitStateUpdate(RenderStateUpdate *stateUpdate)
{
    m_stateUpdates.emplace_back(std::unique_ptr<RenderStateUpdate>(stateUpdate));
}

std::unique_ptr<RenderObject> RenderView::AllocateRenderObject()
{
    return std::make_unique<RenderObject>();
}

void RenderView::Reset()
{
    // Dynamic Render Objects should be recreated every frame.
    m_dynamicRenderObjects.clear();
    m_stateUpdates.clear();
}
