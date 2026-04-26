#pragma once

#include <cstdint>
#include <unordered_map>

#include <Utility/Arena.h>
#include <Utility/Memory.h>
#include <Math/Vectors.h>
#include <Math/Matrix.h>

#include "RenderObject.h"

class RenderView
{
protected:
    Float4x4 m_viewMatrix;
    Float4x4 m_projMatrix;

    std::unordered_map<uint64_t, std::unique_ptr<RenderObject>> m_staticRenderObjects;
    std::unordered_map<uint64_t, std::unique_ptr<RenderObject>> m_dynamicRenderObjects;
    //std::vector<Lightstd::unique_ptr<RenderObject>> m_StaticLightRenderObjects;
    //std::vector<Lightstd::unique_ptr<RenderObject>> m_DynamicLightRenderObjects;
public:

    //void AddStaticLight(Lightstd::unique_ptr<RenderObject> pLightRendObj);
    //void AddDynamicLight(Lightstd::unique_ptr<RenderObject> pLightRendObj);

    void AddStaticRenderObject(uint64_t id, std::unique_ptr<RenderObject> pRendObj);
    void AddDynamicRenderObject(uint64_t id, std::unique_ptr<RenderObject> pRendObj);

    void GetStaticRenderObject(uint64_t id);
    void GetDynamicRenderObject(uint64_t id);

    void RemoveStaticRenderObject(uint64_t id, std::unique_ptr<RenderObject> pRendObj);

    std::unique_ptr<RenderObject> AllocateRenderObject();

    void Reset();
};