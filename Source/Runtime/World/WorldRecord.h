#pragma once

#include <Record/Record.h>
#include <Record/FieldContainers/RecordField.h>
#include <Record/FieldContainers/HashmapField.h>
#include <Record/FieldContainers/BoolField.h>

#include "Chunk/ChunkRecord.h"

class WorldRecord : public Record
{
    Field<HashmapField<uint64_t, RecordField>> f_chunks = {FIELDID(CHNK)};
public:
    static uint32_t StaticType() {return CompMakeRecordType("WRLD");}
};