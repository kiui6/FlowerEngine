#pragma once

#include <cstdint>

enum class RecordFieldType : uint8_t {
    Integer,
    Unsigned,
    Float,
    Double,
    Bool,
    Text,
    String,
};

struct RecordFieldHeader
{
    char fieldID[4] = {'U', 'N', 'K', 'N'};
    RecordFieldType type = RecordFieldType::Integer;
};

class RecordField {
protected:
    RecordFieldHeader m_header;
public:
    
};