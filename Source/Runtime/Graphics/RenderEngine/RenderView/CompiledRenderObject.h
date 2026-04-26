#pragma once

#include "RenderElementTypes.h"
#include "CompiledRenderElement.h"

#include <Math/Matrix.h>

#include <vector>
#include <memory>

struct CompiledRenderObject
{
    glm::mat4x4 m_identityMatrix;
    std::vector<std::unique_ptr<CompiledRenderElement>> m_elements;
public:
    
};