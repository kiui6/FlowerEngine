#pragma once

#include <cstdint>
#include <glm/glm.hpp>

#include <Math/Vectors.h>

#include "RenderElement.h"

class RenderObject
{
    glm::mat4x4 m_identityMatrix;
    std::vector<std::unique_ptr<RenderElement>> m_elements;
    bool m_isStatic = false;
public:

    void SetIdentityMatrix(glm::mat4x4&& matrix) { m_identityMatrix = matrix; }

    void SetDynamic(bool isStatic) {m_isStatic = isStatic;}
    bool IsDynamic() const {return m_isStatic;}

    size_t GetElementsCount() const { return m_elements.size(); }
    std::vector<std::unique_ptr<RenderElement>>& GetElements() { return m_elements; }

    /*
     * Creates RenderElement of specified type, gets ownership over it and returns raw pointer
     * Caller doesn't own the created RenderElement and must not assume ownership
     * Constructed RenderElement will be invalidated by the end of the frame and should not be stored outside of caller function scope
     */
    template <RenderElementClass T>
    T* CreateRenderElement() {
        T* element = new T();
        m_elements.emplace_back(std::unique_ptr<RenderElement>(element));
        return element;
    }

};