#pragma once

#include <Graphics/RenderEngine/RenderPass/RenderPass.h>
#include <Graphics/RenderEngine/GPUContext.h>
#include <Graphics/RenderEngine/RenderState/RenderStateStore.h>

#include <Graphics/RenderElements/OpaqueSpriteRenderElement.h>

#include <Graphics/RenderStates/TilemapRenderState.h>

#include <unordered_map>

#include "OpaqueSpriteElementBatch.h"

class OpaqueRenderPass : public RenderPass
{
    SDL_GPUTexture* m_albedo;
    GPUContext& m_gpu;
    const TilemapRenderState& tilemapState;

    SDL_GPUShader* m_opaqueSpritePipelineVertexShader;
    SDL_GPUShader* m_opaqueSpritePipelineFragmentShader;
    SDL_GPUGraphicsPipeline* m_opaqueSpritePipeline;
    SDL_GPUSampler* m_opaqueSpriteSampler;

    std::unordered_map<uint64_t, OpaqueSpriteElementBatch> m_staticOpaqueSpriteElements;
    std::unordered_map<uint64_t, OpaqueSpriteElementBatch> m_dynamicOpaqueSpriteElements;
public:
    OpaqueRenderPass(GPUContext& context, RenderStateStore& stateStore);
    ~OpaqueRenderPass();

    virtual void Assemble(RenderResourceCompiler& resourceCompiler, RenderObject* object, RenderElement* element) override;
    virtual void Compile(SDL_GPUCommandBuffer* cmd, SDL_GPUCopyPass* copyPass) override;
    virtual void Render(FrameContext& ctx) override;
protected:

    void AssembleOpaqueSpriteRenderElement(RenderResourceCompiler& resourceCompiler, RenderObject* object, OpaqueSpriteRenderElement* element);
};