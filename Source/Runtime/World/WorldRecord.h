#pragma once

#include <Record/Record.h>
#include <Record/FieldContainers/RecordField.h>
#include <Record/FieldContainers/MapField.h>
#include <Record/FieldContainers/TrivialField.h>
#include <Record/FieldContainers/StringField.h>

#include "Chunk/ChunkRecord.h"

class WorldRecord : public Record
{
public:
    // Map of chunk records with packed coordinates [(u64)Key = ((u32)X << 32) | (u32)Y]
    Field<FMap<FLong, FRecord>> Chunks = {FIELDID(CHNK)};
    Field<FString> DisplayName = {FIELDID(DISP)};
    Field<FFloat> TimeSpeedMultiplier = {FIELDID(KTIM)};

    WorldRecord() {SetType(WorldRecord::StaticType());}
    static ID32 StaticType() {return MakeID32("WRLD");}
};