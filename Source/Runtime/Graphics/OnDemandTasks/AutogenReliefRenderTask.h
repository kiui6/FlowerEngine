#pragma once

#include <Graphics/RenderEngine/OnDemand/OnDemandRenderTask.h>

#include <cstdint>
#include <vector>

class AutogenReliefRenderTask : public OnDemandRenderTask
{
public:
    AutogenReliefRenderTask(Texture2DResource& resource, uint16_t width, uint16_t height, unsigned char* data, size_t dataSize) {}
};