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
    Field<FHashmap<uint64_t, FRecord>> Chunks = {FIELDID(CHNK)};
    Field<FString> DisplayName = {FIELDID(DISP)};
    Field<FTrivial<uint16_t>> TimeSpeedMultiplier = {FIELDID(KTIM)};

    WorldRecord() {SetType(WorldRecord::StaticType());}
    static ID32 StaticType() {return MakeID32("WRLD");}
};