#pragma once

#include <Graphics/RenderEngine/RenderPass/RenderPass.h>
#include <Graphics/RenderEngine/GPUContext.h>

#include <unordered_map>
#include <memory>

#include <Debug/DebugWindow.h>
#include <Graphics/RenderStateUpdates/DebugUIStateUpdate.h>

class DebugUIRenderPass : public RenderPass
{
    GPUContext& m_gpu;
    std::shared_ptr<DebugWindow> m_window;
public:
    DebugUIRenderPass(GPUContext& context);
    ~DebugUIRenderPass();

    virtual void Assemble(RenderResourceCompiler& resourceCompiler, RenderObject* object, RenderElement* element) override {}
    virtual void Compile(SDL_GPUCommandBuffer* cmd, SDL_GPUCopyPass* copyPass) override {}
    virtual void Render(FrameContext& ctx) override;
};