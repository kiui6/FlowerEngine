#pragma once

#include <Graphics/RenderEngine/RenderPass/RenderPass.h>

class ReliefRenderPass : public RenderPass
{
public:
    virtual void Compile(RenderResourceCompiler& resourceCompiler, RenderObject* object, RenderElement* element) override {}
    virtual void Render(FrameContext& ctx) override;
};