#pragma once

#include <Record/Record.h>
#include <Record/FieldContainers/RecordField.h>
#include <Record/FieldContainers/ListField.h>
#include <Record/FieldContainers/BoolField.h>

#include <World/Actor/ReferenceRecord.h>

class ChunkRecord : public Record
{
    Field<ListField<RecordField>> f_references = {FIELDID(LREF)};
public:
    static uint32_t StaticType() {return CompMakeRecordType("CHNK");}
};