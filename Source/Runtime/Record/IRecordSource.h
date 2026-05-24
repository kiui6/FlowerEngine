#pragma once

#include <optional>

#include <Utility/ID.h>

#include "RecordMemory.h"
#include "RecordID.h"

class IRecordSource {
public:
    /*
     * Fetches Record memory from file and inserts it into the result
     *
     * @warning
     * This function expects RecordMemory& result to be empty before any operations to allow optimization.
     * If it's not cleared before execution, behavior is undefined. 
     */
    virtual bool FetchRecordMemory(RecordID id, RecordMemory& result) = 0;
    virtual std::optional<ID32> FetchRecordType(RecordID id) = 0;
    virtual bool HasRecord(RecordID id) = 0;
};