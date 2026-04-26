#pragma once

#include <cstdint>
#include <glm/glm.hpp>

#include <Math/Vectors.h>

#include "RenderElement.h"

class RenderObject
{
    glm::mat4x4 m_identityMatrix;
    std::vector<std::unique_ptr<RenderElement>> m_elements;
public:

    void SetIdentityMatrix(glm::mat4x4&& matrix) { m_identityMatrix = matrix; }

};