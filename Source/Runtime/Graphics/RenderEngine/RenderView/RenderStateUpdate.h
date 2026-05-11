#pragma once

#include <vector>
#include <cstdint>

#include <Graphics/RenderEngine/RenderPass/RenderPassTypes.h>

#include "RenderStateUpdateTypes.h"

struct RenderStateUpdate
{
    RenderStateUpdate(RenderPassType initPass, RenderStateUpdateType initType) : renderPass(initPass), type(initType) {}

    inline RenderPassType GetRenderPassType() const {return renderPass;}
    inline RenderStateUpdateType GetUpdateType() const {return type;}
protected:
    RenderPassType renderPass;
    RenderStateUpdateType type;
};