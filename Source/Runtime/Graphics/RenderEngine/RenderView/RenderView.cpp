#include "RenderView.h"

std::unique_ptr<RenderObject> RenderView::AllocateRenderObject()
{
    return std::make_unique<RenderObject>();
}

void RenderView::Reset()
{
    // Dynamic Render Objects should be recreated every frame.
    m_dynamicRenderObjects.clear();
}
