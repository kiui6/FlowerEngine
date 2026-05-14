#pragma once

#include <vector>
#include <cstdint>

#include <Graphics/RenderEngine/RenderState/RenderStateStore.h>

struct RenderStateUpdate
{
    virtual ~RenderStateUpdate() = default;
    
    virtual void Apply(RenderStateStore& store) = 0;
};