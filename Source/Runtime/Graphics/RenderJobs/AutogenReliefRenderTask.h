#pragma once

#include <Graphics/RenderEngine/RenderJob/RenderJob.h>

#include <Graphics/RenderEngine/RenderView/RenderResource.h>

#include <cstdint>
#include <vector>

class AutogenReliefRenderJob : public RenderJob
{
public:
    AutogenReliefRenderJob(Texture2DResource& resource) {}

    virtual void Execute() override {}
};