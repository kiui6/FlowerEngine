#pragma once

#include <cstdint>
#include <glm/glm.hpp>

#include <Math/Vectors.h>

#include <Graphics/RenderElements/OpaqueSpriteRenderElement.h>
#include <Graphics/RenderElements/ReliefSpriteRenderElement.h>

class RenderObject
{
    glm::mat4x4 m_identityMatrix;
    std::vector<OpaqueSpriteRenderElement> m_opaqueSpriteRenderElements;
    std::vector<ReliefSpriteRenderElement> m_reliefSpriteRenderElements;
public:

    void SetIdentityMatrix(glm::mat4x4&& matrix) { m_identityMatrix = matrix; }

    /*
     * Creates RenderElement of specified type, gets ownership over it and returns raw pointer
     * Caller doesn't own the created RenderElement and must not assume ownership
     * Constructed RenderElement will be invalidated by the end of the frame and should not be stored outside of caller function scope
     */
    OpaqueSpriteRenderElement& CreateOpaqueSpriteRenderElement() {
        return m_opaqueSpriteRenderElements.emplace_back(OpaqueSpriteRenderElement{});
    }
    const std::vector<OpaqueSpriteRenderElement>& GetOpaqueSpriteElements() const {return m_opaqueSpriteRenderElements;}

};