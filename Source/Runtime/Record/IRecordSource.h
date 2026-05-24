#pragma once

#include <optional>

#include <Utility/ID.h>

#include "RecordMemory.h"
#include "RecordID.h"

class IRecordSource {
public:
    virtual std::optional<RecordMemory> FetchRecordMemory(RecordID id) = 0;
    virtual std::optional<ID32> FetchRecordType(RecordID id) = 0;
    virtual bool HasRecord(RecordID id) = 0;
};