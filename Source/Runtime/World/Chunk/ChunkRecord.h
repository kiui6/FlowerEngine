#pragma once

#include <Record/Record.h>
#include <Record/FieldContainers/RecordField.h>
#include <Record/FieldContainers/ListField.h>
#include <Record/FieldContainers/TrivialField.h>

#include <World/Actor/ReferenceRecord.h>

class ChunkRecord : public Record
{
    Field<FList<FRecord>> f_references = {FIELDID(LREF)};
public:
    ChunkRecord() {SetType(ChunkRecord::StaticType());}
    static ID32 StaticType() {return MakeID32("CHNK");}
};