#pragma once

#include "RecordMemory.h"
#include "RecordID.h"

class IRecordSource {
public:
    virtual RecordMemory FetchRecordMemory(RecordID id) = 0;
    virtual bool HasRecord(RecordID id) = 0;
};