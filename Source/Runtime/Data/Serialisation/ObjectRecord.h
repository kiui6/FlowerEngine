#pragma once

#include <cstdint>
#include <vector>
#include <memory>

#include <Utility/Record.h>

#include "ObjectField.h"

struct ObjectRecordHeader
{
    uint32_t objectType = CompMakeRecordType("UNKN");
    uint64_t objectID = 0;
    uint16_t fieldCount = 0;
};

class ObjectRecord
{
    ObjectRecordHeader m_header;
    std::vector<std::unique_ptr<ObjectRecordField>> m_fields;
public:

};