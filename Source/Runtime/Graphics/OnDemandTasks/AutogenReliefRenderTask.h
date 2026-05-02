#pragma once

#include <Graphics/RenderEngine/OnDemand/OnDemandRenderTask.h>

#include <cstdint>
#include <vector>

class AutogenReliefRenderTask : public OnDemandRenderTask
{
public:
    AutogenReliefRenderTask(uint16_t width, uint16_t height, std::vector<std::byte>& data) {}
};