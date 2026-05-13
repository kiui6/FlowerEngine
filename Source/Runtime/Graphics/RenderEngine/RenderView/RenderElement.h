#pragma once

#include <vector>
#include <cstdint>
#include <concepts>

#include "RenderResource.h"
#include "RenderElementTypes.h"

#include <Graphics/RenderEngine/RenderPass/RenderPassTypes.h>

struct RenderElement
{
    RenderElement(RenderPassType initRenderPass, RenderElementType initGeoType) 
        : renderPass(initRenderPass), geometryType(initGeoType) {}

    inline RenderPassType GetRenderPassType() const {return renderPass;}
    inline RenderElementType GetRenderElementType() const {return geometryType;}
protected:
    RenderPassType renderPass;
    RenderElementType geometryType;
};

template<typename T>
concept RenderElementClass = std::is_base_of<RenderElement, T>::value;