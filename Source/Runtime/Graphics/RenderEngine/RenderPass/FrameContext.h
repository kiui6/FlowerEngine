#pragma once

#include <vector>

#include <Graphics/RenderEngine/RenderView/CompiledRenderElement.h>
#include <Graphics/RenderEngine/RenderView/CompiledRenderResource.h>

class RenderEngine;

class FrameContext {
    std::vector<CompiledRenderElement> m_elements;
    RenderEngine* m_engine;
public:

    
};