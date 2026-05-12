#pragma once

#include <Graphics/RenderEngine/RenderPass/RenderPass.h>

class ReliefRenderPass : public RenderPass
{
    SDL_GPUTexture* m_relief;
    GPUContext& m_gpu;
public:
    ReliefRenderPass(GPUContext& context);
    ~ReliefRenderPass();

    virtual void UpdateState(RenderStateUpdate* updateObj) override {}
    virtual void Assemble(RenderResourceCompiler& resourceCompiler, RenderObject* object, RenderElement* element) override {}
    virtual void Compile(SDL_GPUCommandBuffer* cmd, SDL_GPUCopyPass* copyPass) override {}
    virtual void Render(FrameContext& ctx) override;
};