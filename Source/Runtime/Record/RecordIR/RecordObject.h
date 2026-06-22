#pragma once

#include "FieldNode.h"
#include <cstdint>
#include <string>



class RecordObject
{
    uint64_t m_id = 0;
    bool m_deleted = false;
    bool m_delta = false;
    std::unordered_map<std::string, FieldNode*> m_fields;
public:

};