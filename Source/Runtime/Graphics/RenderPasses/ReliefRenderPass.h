#pragma once

#include <Graphics/RenderEngine/RenderPass/RenderPass.h>

class ReliefRenderPass : public RenderPass
{
    SDL_GPUTexture* m_relief;
    GPUContext& m_gpu;
public:
    ReliefRenderPass(GPUContext& context);
    ~ReliefRenderPass();

    virtual void CompileStaticObject(RenderResourceCompiler& resourceCompiler, const RenderObject& object) override {}
    virtual void CompileDynamicObject(RenderResourceCompiler& resourceCompiler, const RenderObject& object) override {}
    virtual void PrepareFrame(SDL_GPUCommandBuffer* cmd, SDL_GPUCopyPass* copyPass) override {}
    virtual void RenderFrame(FrameContext& ctx) override;
    virtual void Cleanup() override {}
};