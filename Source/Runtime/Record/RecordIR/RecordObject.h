#pragma once

#include "FieldNode.h"
#include <cstdint>

#include <Utility/Containers/FlatHashMap.h>
#include <Utility/ID.h>


class RecordObject
{
    uint64_t m_id = 0;
    bool m_deleted = false;
    bool m_delta = false;
    FlatHashMap<ID32, FieldNode*> m_fields;
public:

    inline void PushField(ID32 field, FieldNode* node) {m_fields.Emplace(field, node);}

    inline void Reset() {
        m_id = 0;
        m_deleted = false;
        m_delta = false;
        m_fields.Clear();
    }
};