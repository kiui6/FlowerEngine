#pragma once

#include <Graphics/RenderEngine/RenderPass/RenderPass.h>
#include <Graphics/RenderEngine/GPUContext.h>

#include <Graphics/RenderElements/OpaqueSpriteRenderElement.h>

#include <unordered_map>

#include "OpaqueSpriteElementBatch.h"

class OpaqueRenderPass : public RenderPass
{
    SDL_GPUTexture* m_albedo;
    GPUContext& m_gpu;

    SDL_GPUShader* m_opaqueSpritePipelineVertexShader;
    SDL_GPUShader* m_opaqueSpritePipelineFragmentShader;
    SDL_GPUGraphicsPipeline* m_opaqueSpritePipeline;
    SDL_GPUSampler* m_opaqueSpriteSampler;

    std::unordered_map<uint64_t, OpaqueSpriteElementBatch> m_staticOpaqueSpriteElements;
    std::unordered_map<uint64_t, OpaqueSpriteElementBatch> m_dynamicOpaqueSpriteElements;
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