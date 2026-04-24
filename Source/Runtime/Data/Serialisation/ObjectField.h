#pragma once

#include <cstdint>

enum class ObjectRecordFieldType : uint8_t {
    Integer,
    Unsigned,
    Float,
    Double,
    Bool,
    Text,
    String,
};

struct ObjectRecordFieldHeader
{
    char fieldID[4] = {'U', 'N', 'K', 'N'};
    ObjectRecordFieldType type = ObjectRecordFieldType::Integer;
};

class ObjectRecordField {
protected:
    ObjectRecordFieldHeader m_header;
public:
    
};