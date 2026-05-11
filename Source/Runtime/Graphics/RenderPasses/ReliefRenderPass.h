#pragma once

#include <Graphics/RenderEngine/RenderPass/RenderPass.h>

class ReliefRenderPass : public RenderPass
{
    SDL_GPUTexture* m_relief;
    GPUContext& m_gpu;
public:
    ReliefRenderPass(GPUContext& context);
    ~ReliefRenderPass();

    virtual void Compile(RenderResourceCompiler& resourceCompiler, RenderObject* object, RenderElement* element) override {}
    virtual void Render(FrameContext& ctx) override;
};