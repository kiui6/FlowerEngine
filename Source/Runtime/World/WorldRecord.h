#pragma once

#include <Record/Record.h>
#include <Record/FieldContainers/RecordField.h>
#include <Record/FieldContainers/HashmapField.h>
#include <Record/FieldContainers/BoolField.h>
#include <Record/FieldContainers/IntField.h>
#include <Record/FieldContainers/StringField.h>

#include "Chunk/ChunkRecord.h"

class WorldRecord : public Record
{
public:
    Field<HashmapField<uint64_t, RecordField>> Chunks = {FIELDID(CHNK)};
    Field<StringField> DisplayName = {FIELDID(DISP)};
    Field<UInt16Field> TimeSpeedMultiplier = {FIELDID(KTIM)};

    WorldRecord() {SetType(WorldRecord::StaticType());}
    static ID32 StaticType() {return MakeID32("WRLD");}
};