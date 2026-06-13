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
        RenderObject object;
        // How many frames ago was this object referenced. 0 meaning it's referenced in current frame and should be rendered
        uint32_t lastReferencedFramesAgo = 0;
    };

    struct RenderStateUpdateHandle {
        RenderStateUpdateHandle(std::unique_ptr<RenderStateUpdate> obj, bool isRef) : object(std::move(obj)), isReferenced(isRef) {}

        std::unique_ptr<RenderStateUpdate> object;
        bool isReferenced = false;
    };
protected:
    Float4x4 m_projMatrix{};
    Float3 m_cameraPosition{};

    std::unordered_map<uint64_t, RenderObjectHandle> m_staticRenderObjects{};
    bool m_staticRenderObjectsDirty = false;
    std::vector<RenderObject> m_dynamicRenderObjects{};

    std::unordered_map<uint64_t, RenderStateUpdateHandle> m_stateUpdates{};
    std::set<std::unique_ptr<RenderJob>> m_renderJobs{};
public:
    void SetCameraPosition(Float3 position) {m_cameraPosition = position;}
    Float3 GetCameraPosition() const {return m_cameraPosition;}

    // Creates or retrieves RenderStateUpdate object and references it. Render state updates are performed before any job, compilation or rendering.
    template <RenderStateUpdateClass T>
    T* GetStateUpdate();

    // Submits render job to the render view. Render jobs are executed before any compilation or rendering.
    void SubmitJob(RenderJob* job);

    RenderObject& AddDynamicRenderObject(uint64_t id);

    RenderObject& AddStaticRenderObject(uint64_t id);
    RenderObject* GetStaticRenderObject(uint64_t id);
    bool HasStaticRenderObject(uint64_t id);

    void RemoveStaticRenderObject(uint64_t id);

    void Reset();
};

template <RenderStateUpdateClass T>
inline T *RenderView::GetStateUpdate()
{
    auto it = m_stateUpdates.find(T::StaticType());
    if(it != m_stateUpdates.end()) {
        it->second.isReferenced = true;
        return static_cast<T*>(it->second.object.get());
    }

    auto cond = m_stateUpdates.emplace(T::StaticType(), RenderStateUpdateHandle{std::make_unique<T>(), true});

    return static_cast<T*>(cond.first->second.object.get());
}
