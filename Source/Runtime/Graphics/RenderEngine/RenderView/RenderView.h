#pragma once

#include <cstdint>
#include <unordered_map>

#include <Utility/Memory.h>
#include <Math/Vectors.h>
#include <Math/Matrix.h>

#include "RenderObject.h"
#include "RenderStateUpdate.h"

class RenderView
{
    friend class RenderEngine;

    struct StaticRenderObjectHandle {
        std::shared_ptr<RenderObject> object;
        bool referenced = false;
    };
protected:
    Float4x4 m_viewMatrix;
    Float4x4 m_projMatrix;

    std::unordered_map<uint64_t, StaticRenderObjectHandle> m_staticRenderObjects;
    bool m_staticRenderObjectsDirty = false;
    std::unordered_map<uint64_t, std::unique_ptr<RenderObject>> m_dynamicRenderObjects;

    std::vector<std::unique_ptr<RenderStateUpdate>> m_stateUpdates;
public:
    RenderObject* GetDynamicRenderObject(uint64_t id);

    std::weak_ptr<RenderObject> GetStaticRenderObject(uint64_t id);

    void RemoveStaticRenderObject(uint64_t id, std::unique_ptr<RenderObject> pRendObj);

    void SubmitStateUpdate(RenderStateUpdate* stateUpdate);


    void Reset();
};