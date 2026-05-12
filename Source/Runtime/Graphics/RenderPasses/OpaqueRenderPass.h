#pragma once

#include <Graphics/RenderEngine/RenderPass/RenderPass.h>
#include <Graphics/RenderEngine/GPUContext.h>

#include <Graphics/RenderElements/OpaqueSpriteRenderElement.h>

#include <unordered_map>

class OpaqueRenderPass : public RenderPass
{
    SDL_GPUTexture* m_albedo;
    GPUContext& m_gpu;

    SDL_GPUSampler* m_opaqueSpriteSampler;
    std::unordered_map<uint64_t, CompiledOpaqueSpriteRenderElement> m_opaqueSpriteElements;
public:
    OpaqueRenderPass(GPUContext& context);
    ~OpaqueRenderPass();

    virtual void UpdateState(RenderStateUpdate* updateObj) override {}
    virtual void Assemble(RenderResourceCompiler& resourceCompiler, RenderObject* object, RenderElement* element) override;
    virtual void Compile(SDL_GPUCommandBuffer* cmd, SDL_GPUCopyPass* copyPass) override;
    virtual void Render(FrameContext& ctx) override;
protected:

    void AssembleOpaqueSpriteRenderElement(RenderResourceCompiler& resourceCompiler, RenderObject* object, OpaqueSpriteRenderElement* element);
};