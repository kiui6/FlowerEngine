#pragma once

#include <Record/Record.h>
#include <Record/FieldContainers/RecordField.h>
#include <Record/FieldContainers/BoolField.h>

class AtlasRecord : public Record
{
public:
    AtlasRecord() {SetType(AtlasRecord::StaticType());}

    Field<RecordField> AlbedoTexture {FIELDID(ALBR)};

    Field<BoolField> AutoReliefGeneration {FIELDID(RLFA)};
    Field<RecordField> ReliefTexture {FIELDID(RLFR)};

    static ID32 StaticType() {return MakeID32("ATLS");}
};