#pragma once

#include <vector>
#include <cstdint>

#include "RenderResource.h"
#include "CompiledRenderElement.h"
#include "RenderElementTypes.h"

#include <Graphics/RenderEngine/RenderPass/RenderPassTypes.h>

struct RenderElement
{
    RenderPassType renderPass;
    RenderElementType geometryType;

    RenderElement(RenderPassType initRenderPass, RenderElementType initGeoType) 
        : renderPass(initRenderPass), geometryType(initGeoType) {}
    virtual ~RenderElement() {}

    virtual CompiledRenderElement* CreateCompiledElement();
};