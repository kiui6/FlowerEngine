#pragma once

#include <Record/Record.h>
#include <Record/FieldContainers/RecordField.h>
#include <Record/FieldContainers/HashmapField.h>
#include <Record/FieldContainers/TrivialField.h>
#include <Record/FieldContainers/StringField.h>

#include "Chunk/ChunkRecord.h"

class WorldRecord : public Record
{
public:
    Field<FHashmap<FUInt64, FRecord>> Chunks = {FIELDID(CHNK)};
    Field<FString> DisplayName = {FIELDID(DISP)};
    Field<FUInt16> TimeSpeedMultiplier = {FIELDID(KTIM)};

    WorldRecord() {SetType(WorldRecord::StaticType());}
    static ID32 StaticType() {return MakeID32("WRLD");}
};