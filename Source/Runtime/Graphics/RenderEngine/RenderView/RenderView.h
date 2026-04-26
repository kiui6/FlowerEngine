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
    ArenaAllocator<Kilobytes(256)> m_rendObjArena;

protected:
    Float4x4 m_viewMatrix;
    Float4x4 m_projMatrix;

    std::unordered_map<uint64_t, RenderObject*> m_staticRenderObjects;
    std::unordered_map<uint64_t, RenderObject*> m_dynamicRenderObjects;
    //std::vector<LightRenderObject*> m_StaticLightRenderObjects;
    //std::vector<LightRenderObject*> m_DynamicLightRenderObjects;
public:

    //void AddStaticLight(LightRenderObject* pLightRendObj);
    //void AddDynamicLight(LightRenderObject* pLightRendObj);

    void AddStaticRenderObject(uint64_t id, RenderObject* pRendObj);
    void AddDynamicRenderObject(uint64_t id, RenderObject* pRendObj);

    void GetStaticRenderObject(uint64_t id);
    void GetDynamicRenderObject(uint64_t id);

    void RemoveStaticRenderObject(uint64_t id, RenderObject* pRendObj);
    void RemoveDynamicRenderObject(uint64_t id, RenderObject* pRendObj);

    RenderObject* AllocateRenderObject();
};