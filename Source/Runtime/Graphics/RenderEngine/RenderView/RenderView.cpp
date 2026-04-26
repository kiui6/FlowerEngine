#include "RenderView.h"

RenderObject *RenderView::AllocateRenderObject()
{
    return m_rendObjArena.AllocateObject<RenderObject>();
}