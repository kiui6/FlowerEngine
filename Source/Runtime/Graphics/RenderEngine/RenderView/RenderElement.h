#pragma once

#include <vector>
#include <cstdint>

#include "RenderResource.h"
#include "CompiledRenderElement.h"
#include "RenderElementTypes.h"

struct RenderElement
{
    RenderElementType type;

    RenderElement(RenderElementType initType) : type(initType) {}
    virtual ~RenderElement() {}

    virtual CompiledRenderElement* CreateCompiledElement();
};