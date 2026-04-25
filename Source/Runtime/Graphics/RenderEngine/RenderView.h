#pragma once

#include <cstdint>

class RenderView
{

public:
    void AddRenderObject();

    void GetRenderObject(uint64_t id);
};