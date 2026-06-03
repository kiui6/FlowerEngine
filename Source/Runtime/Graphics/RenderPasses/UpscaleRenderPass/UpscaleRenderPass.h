#pragma once

#include <Graphics/RenderEngine/RenderPass/RenderPass.h>
#include <Graphics/RenderEngine/RenderConstants.h>

#include <Graphics/RenderStates/UpscaleRenderState.h>
#include <Graphics/RenderEngine/RenderState/RenderStateStore.h>

class UpscaleRenderPass : public RenderPass
{
    GPUContext& m_gpu;
    const UpscaleRenderState& m_state;

    SDL_GPUBuffer* m_upscaleBuffer;
    SDL_GPUTransferBuffer* m_upscaleTransferBuffer = nullptr;
    bool m_upscaleBufferDirty = false;

    SDL_GPUShader* m_vertexShader = nullptr;
    SDL_GPUShader* m_fragmentShader = nullptr;
    SDL_GPUGraphicsPipeline* m_pipeline = nullptr;
    SDL_GPUSampler* m_sampler;
public:
    UpscaleRenderPass(GPUContext& context, RenderStateStore& stateStore);
    ~UpscaleRenderPass();

    virtual void Compile(RenderResourceCompiler& resourceCompiler, RenderObject* object, RenderElement* element) override {}
    virtual void Prepare(SDL_GPUCommandBuffer* cmd, SDL_GPUCopyPass* copyPass) override;
    virtual void Render(FrameContext& ctx) override;
    virtual void Cleanup() override {}

protected:
    void CreateRenderPipeline();
    void OnStateChange();
};