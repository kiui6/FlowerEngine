#pragma once

#include <vector>
#include <cstdint>

#include <Graphics/RenderEngine/RenderPass/RenderPassTypes.h>

#include "RenderStateUpdateTypes.h"

struct RenderStateUpdate
{
    RenderStateUpdate(std::vector<RenderPassType> initPasses, RenderStateUpdateType initType) : renderPasses(initPasses), type(initType) {}

    inline const std::vector<RenderPassType>& GetRenderPassTypes() const {return renderPasses;}
    inline RenderStateUpdateType GetUpdateType() const {return type;}
protected:
    std::vector<RenderPassType> renderPasses;
    RenderStateUpdateType type;
};