#pragma once

#include <vector>
#include <cstdint>

#include "RenderStateUpdateTypes.h"

struct RenderStateUpdate
{
    RenderStateUpdateType type;

    RenderStateUpdate(RenderStateUpdateType initType) : type(initType) {}
};