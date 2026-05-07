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
    static ID32 StaticType() {return MakeID32("WRLD");}
};