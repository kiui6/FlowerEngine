#pragma once

#include <Record/Record.h>
#include <Record/FieldContainers/StringField.h>

class TextureRecord : public Record
{
public:
    TextureRecord() {SetType(TextureRecord::StaticType());}

    Field<FString> TexturePath {FIELDID(TEXP)};

    static ID32 StaticType() {return MakeID32("TEX2");}
};