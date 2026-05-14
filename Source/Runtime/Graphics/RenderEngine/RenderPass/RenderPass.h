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
    virtual void Assemble(RenderResourceCompiler& resourceCompiler, RenderObject* object, RenderElement* element) = 0;
    virtual void Compile(SDL_GPUCommandBuffer* cmd, SDL_GPUCopyPass* copyPass) = 0;
    virtual void Render(FrameContext& ctx) = 0;
};