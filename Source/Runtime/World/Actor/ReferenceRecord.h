#pragma once

#include <Record/Record.h>
#include <Record/FieldContainers/RecordField.h>
#include <Record/FieldContainers/BoolField.h>

class ReferenceRecord : public Record
{
    Field<BoolField> f_isDynamic = {FIELDID(DYNA)};
    Field<RecordField> f_parentChunk = {FIELDID(CHNK)};
public:
    static uint32_t StaticType() {return CompMakeRecordType("REFR");}
};