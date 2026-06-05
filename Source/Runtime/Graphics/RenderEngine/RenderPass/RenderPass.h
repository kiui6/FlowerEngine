#pragma once

#include <Graphics/RenderEngine/ResourceCompiler.h>
#include <Graphics/RenderEngine/FrameContext.h>
#include <Graphics/RenderEngine/RenderUtils.h>

#include <SDL3/SDL_gpu.h>

class CompiledRenderElement;
class RenderElement;
class RenderObject;

class RenderPass
{
public:
    virtual ~RenderPass() = default;
    virtual void CompileStaticObject(RenderResourceCompiler& resourceCompiler, const RenderObject& object) = 0;
    virtual void CompileDynamicObject(RenderResourceCompiler& resourceCompiler, const RenderObject& object) = 0;
    virtual void PrepareFrame(SDL_GPUCommandBuffer* cmd, SDL_GPUCopyPass* copyPass) = 0;
    virtual void RenderFrame(FrameContext& ctx) = 0;
    virtual void Cleanup() = 0;
};