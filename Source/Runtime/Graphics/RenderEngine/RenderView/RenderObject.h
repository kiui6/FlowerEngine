#pragma once

#include <cstdint>
#include <glm/glm.hpp>

#include <Math/Vectors.h>

#include "CompiledRenderObject.h"
#include "RenderElement.h"

class RenderObject
{
    CompiledRenderObject* m_pCompiledRO;

    glm::mat4x4 m_identityMatrix;
public:

    void SetIdentityMatrix(glm::mat4x4&& matrix) { m_identityMatrix = matrix; }
    
    void GetCompiledObject();
};