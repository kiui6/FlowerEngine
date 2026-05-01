#pragma once

#include <Record/Record.h>
#include <Record/FieldContainers/StringField.h>
#include <Record/FieldContainers/BoolField.h>

class TextureRecord : public Record
{
public:
    Field<StringField> TexturePath {FIELDID(TEXP)};

    static uint32_t StaticType() {return CompMakeRecordType("TEX2");}
};