#pragma once

#include <Graphics/RenderEngine/RenderPass/RenderPass.h>

class UpscaleRenderPass : public RenderPass
{
    SDL_GPUBuffer* m_triangleVertBuffer;
public:
    UpscaleRenderPass(GPUContext& context);
    ~UpscaleRenderPass();

    virtual void UpdateState(RenderStateUpdate* updateObj) override {}
    virtual void Assemble(RenderResourceCompiler& resourceCompiler, RenderObject* object, RenderElement* element) override {}
    virtual void Compile() override {}
    virtual void Render(FrameContext& ctx) override;
};