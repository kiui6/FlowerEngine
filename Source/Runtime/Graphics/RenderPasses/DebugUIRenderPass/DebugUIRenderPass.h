#pragma once

#include <Graphics/RenderEngine/RenderPass/RenderPass.h>
#include <Graphics/RenderEngine/GPUContext.h>

#include <unordered_map>
#include <memory>

#include <Debug/DebugWindow.h>
#include <Graphics/RenderEngine/RenderState/RenderStateStore.h>
#include <Graphics/RenderStates/DebugUIRenderState.h>

class DebugUIRenderPass : public RenderPass
{
    GPUContext& m_gpu;
    const DebugUIRenderState& m_state;
public:
    DebugUIRenderPass(GPUContext& context, RenderStateStore& stateStore);
    ~DebugUIRenderPass();

    virtual void CompileStaticObject(RenderResourceCompiler& resourceCompiler, const RenderObject& object) override {}
    virtual void CompileDynamicObject(RenderResourceCompiler& resourceCompiler, const RenderObject& object) override {}
    virtual void PrepareFrame(SDL_GPUCommandBuffer* cmd, SDL_GPUCopyPass* copyPass) override {}
    virtual void RenderFrame(FrameContext& ctx) override;
    virtual void Cleanup() override {}

private:
    void cbOnChange();
};