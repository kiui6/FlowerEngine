#pragma once

#include <cstdint>
#include <vector>
#include <memory>

#include "Field.h"

struct RecordHeader
{
    char type[4] = {'U', 'N', 'K', 'N'};
    uint64_t objectID = 0;
    uint16_t fieldCount = 0;
};

class Record
{
    RecordHeader m_header;
    std::vector<std::unique_ptr<RecordField>> m_fields;
public:

};