#pragma once

#include "RenderElementTypes.h"

class CompiledRenderObject;

struct CompiledRenderElement
{
    RenderElementType type;
    CompiledRenderObject* renderObject;

    CompiledRenderElement(RenderElementType initType) : type(initType) {}
};