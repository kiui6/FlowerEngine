#pragma once

#include <Graphics/RenderEngine/RenderPass/RenderPass.h>
#include <Graphics/RenderEngine/GPUContext.h>
#include <Graphics/RenderEngine/RenderState/RenderStateStore.h>

#include <Graphics/RenderElements/OpaqueSpriteRenderElement.h>

#include <Graphics/RenderStates/TilemapRenderState.h>
#include <Graphics/RenderStates/GlobalRenderState.h>

#include <unordered_map>

#include "OpaqueSpriteElementBatch.h"

class OpaqueRenderPass : public RenderPass
{
    std::array<SDL_GPUTexture*, FRAMES_IN_FLIGHT> m_albedo;
    GPUContext& m_gpu;

    const GlobalRenderState& globalState;
    const TilemapRenderState& tilemapState;

    SDL_GPUBuffer* m_quadVertexBuffer;

    SDL_GPUShader* m_opaqueSpritePipelineVertexShader;
    SDL_GPUShader* m_opaqueSpritePipelineFragmentShader;
    SDL_GPUGraphicsPipeline* m_opaqueSpritePipeline;
    SDL_GPUSampler* m_opaqueSpriteSampler;

    std::unordered_map<uint64_t, OpaqueSpriteElementBatch> m_staticOpaqueSpriteElements;
    std::unordered_map<uint64_t, OpaqueSpriteElementBatch> m_dynamicOpaqueSpriteElements;
public:
    OpaqueRenderPass(GPUContext& context, RenderStateStore& stateStore);
    ~OpaqueRenderPass();

    virtual void Compile(RenderResourceCompiler& resourceCompiler, RenderObject* object, RenderElement* element) override;
    virtual void Prepare(SDL_GPUCommandBuffer* cmd, SDL_GPUCopyPass* copyPass) override;
    virtual void Render(FrameContext& ctx) override;
    virtual void Cleanup() override;
protected:
    void CreateOpaqueSpritePipeline();

    void CompileOpaqueSpriteRenderElement(RenderResourceCompiler& resourceCompiler, RenderObject* object, OpaqueSpriteRenderElement* element);
};