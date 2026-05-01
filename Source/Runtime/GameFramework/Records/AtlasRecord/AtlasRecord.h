#pragma once

#include <Record/Record.h>
#include <Record/FieldContainers/RecordField.h>
#include <Record/FieldContainers/BoolField.h>

class AtlasRecord : public Record
{
public:
    Field<RecordField> AlbedoTexture {FIELDID(ALBR)};

    Field<BoolField> AutoReliefGeneration {FIELDID(RLFA)};
    Field<RecordField> ReliefTexture {FIELDID(RLFR)};

    static uint32_t StaticType() {return CompMakeRecordType("ATLS");}
};