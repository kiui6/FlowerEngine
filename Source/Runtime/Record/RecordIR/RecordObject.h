#pragma once

#include <Utility/Memory/Arena.h>

class RecordObject
{
    DynamicArena<16*1024> m_arena{};
    uint64_t m_id = 0;
    bool m_deleted = false;
    bool m_delta = false;
public:

};