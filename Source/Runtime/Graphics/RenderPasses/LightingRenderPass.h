#pragma once

#include <Graphics/RenderEngine/RenderPass/RenderPass.h>

class LightingRenderPass : public RenderPass
{
public:
    LightingRenderPass(GPUContext& context);
    ~LightingRenderPass();

    virtual void Compile(RenderResourceCompiler& resourceCompiler, RenderObject* object, RenderElement* element) override {}
    virtual void Prepare(SDL_GPUCommandBuffer* cmd, SDL_GPUCopyPass* copyPass) override {}
    virtual void Render(FrameContext& ctx) override;
    virtual void Cleanup() override {}
};