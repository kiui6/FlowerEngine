#pragma once

#include <cstdint>
#include <unordered_map>
#include <set>

#include <Utility/Memory.h>
#include <Math/Vectors.h>
#include <Math/Matrix.h>

#include "RenderObject.h"
#include <Graphics/RenderEngine/RenderState/RenderStateUpdate.h>
#include <Graphics/RenderEngine/RenderJob/RenderJob.h>

// TODO: Make all RenderObjects & their elements reusable. Ideally allocate through custom allocator.

class RenderView
{
    friend class RenderEngine;

    struct RenderObjectHandle {
        std::shared_ptr<RenderObject> object;
        // How many frames ago was this object referenced. 0 meaning it's referenced in current frame and should be rendered
        uint32_t lastReferencedFramesAgo = 0;
    };
protected:
    Float4x4 m_viewMatrix;
    Float4x4 m_projMatrix;

    std::unordered_map<uint64_t, RenderObjectHandle> m_staticRenderObjects;
    bool m_staticRenderObjectsDirty = false;
    std::unordered_map<uint64_t, std::unique_ptr<RenderObject>> m_dynamicRenderObjects;

    std::set<std::unique_ptr<RenderStateUpdate>> m_stateUpdates;
    std::set<std::unique_ptr<RenderJob>> m_renderJobs;
public:
    // Submits render state update to the render view. Render state updates are performed before any job, compilation or rendering.
    void SubmitStateUpdate(RenderStateUpdate* stateUpdate);

    // Submits render job to the render view. Render jobs are executed before any compilation or rendering.
    void SubmitJob(RenderJob* job);

    RenderObject* GetDynamicRenderObject(uint64_t id);

    RenderObject* AddStaticRenderObject(uint64_t id);
    RenderObject* GetStaticRenderObject(uint64_t id);
    bool HasStaticRenderObject(uint64_t id);

    void RemoveStaticRenderObject(uint64_t id);

    void Reset();
};