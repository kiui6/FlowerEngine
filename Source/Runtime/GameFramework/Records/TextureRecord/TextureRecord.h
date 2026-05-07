#pragma once

#include <Record/Record.h>
#include <Record/FieldContainers/StringField.h>
#include <Record/FieldContainers/BoolField.h>

class TextureRecord : public Record
{
public:
    TextureRecord() {SetType(TextureRecord::StaticType());}

    Field<StringField> TexturePath {FIELDID(TEXP)};

    static ID32 StaticType() {return MakeID32("TEX2");}
};