#pragma once

#include <Graphics/RenderEngine/RenderPass/RenderPass.h>

class LightingRenderPass : public RenderPass
{
public:
    LightingRenderPass(GPUContext& context);
    ~LightingRenderPass();

    virtual void UpdateState(RenderStateUpdate* updateObj) override {}
    virtual void Assemble(RenderResourceCompiler& resourceCompiler, RenderObject* object, RenderElement* element) override {}
    virtual void Compile() override {}
    virtual void Render(FrameContext& ctx) override;
};