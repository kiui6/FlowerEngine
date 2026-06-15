#pragma once

#include <Record/Record.h>
#include <Record/FieldContainers/RecordField.h>
#include <Record/FieldContainers/TrivialField.h>

class AtlasRecord : public Record
{
public:
    AtlasRecord() {SetType(AtlasRecord::StaticType());}

    Field<FRecord> AlbedoTexture {FIELDID(ALBR)};

    Field<FTrivial<bool>> AutoReliefGeneration {FIELDID(RLFA)};
    Field<FRecord> ReliefTexture {FIELDID(RLFR)};

    static ID32 StaticType() {return MakeID32("ATLS");}
};