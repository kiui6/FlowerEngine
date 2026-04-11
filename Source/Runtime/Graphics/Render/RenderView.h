#pragma once

#include <vector>

class RenderObject;
class LightRenderObject;

class RenderView
{
protected:
    std::vector<RenderObject*> m_RenderObjects;
    std::vector<LightRenderObject*> m_StaticLightRenderObjects;
    std::vector<LightRenderObject*> m_DynamicLightRenderObjects;
public:

    void AddStaticLight(LightRenderObject* pLightRendObj);
    void AddDynamicLight(LightRenderObject* pLightRendObj);

    void AddRenderObject(RenderObject* pRendObj);

};