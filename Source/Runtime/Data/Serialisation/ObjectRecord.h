#pragma once

#include <cstdint>
#include <vector>
#include <memory>

#include <Utility/ID.h>

#include "ObjectField.h"

struct ObjectRecordHeader
{
    ID32 objectType = MakeID32("UNKN");
    uint64_t objectID = 0;
    uint16_t fieldCount = 0;
};

class ObjectRecord
{
    ObjectRecordHeader m_header;
    std::vector<std::unique_ptr<ObjectRecordField>> m_fields;
public:

};