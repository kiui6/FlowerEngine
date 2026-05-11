#pragma once

#include <Graphics/RenderEngine/RenderPass/RenderPass.h>

class UpscaleRenderPass : public RenderPass
{
public:
    UpscaleRenderPass(GPUContext& context);
    ~UpscaleRenderPass();

    virtual void Compile(RenderResourceCompiler& resourceCompiler, RenderObject* object, RenderElement* element) override {}
    virtual void Render(FrameContext& ctx) override;
};