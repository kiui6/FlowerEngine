#pragma once

#include "RenderView/RenderResourceContainer.h"
#include "RenderView/RenderResource.h"
#include "GPUContext.h"

class RenderResourceCompiler {
    CompiledRenderResources& m_resources;
    GPUContext& m_ctx;
public:
    RenderResourceCompiler(GPUContext& ctx, CompiledRenderResources& resources) : m_ctx(ctx), m_resources(resources) {}

    int CompileTexture2D(Texture2DResource& tex2d);
};