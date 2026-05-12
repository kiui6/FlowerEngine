#pragma once

#include <Graphics/RenderEngine/RenderPass/RenderPass.h>
#include <Graphics/RenderEngine/GPUContext.h>

class OpaqueRenderPass : public RenderPass
{
    SDL_GPUTexture* m_albedo;
    GPUContext& m_gpu;
public:
    OpaqueRenderPass(GPUContext& context);
    ~OpaqueRenderPass();

    virtual void UpdateState(RenderStateUpdate* updateObj) override {}
    virtual void Compile(RenderResourceCompiler& resourceCompiler, RenderObject* object, RenderElement* element) override;
    virtual void Render(FrameContext& ctx) override;
};