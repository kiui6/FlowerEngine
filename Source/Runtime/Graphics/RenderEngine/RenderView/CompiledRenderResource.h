#pragma once

#include "RenderResourceType.h"

class CompiledRenderResource
{
    CompiledRenderResource(RenderResourceType initType) 
        : type(initType) {}

    uint64_t id = 0;
protected:
    RenderResourceType type;
};