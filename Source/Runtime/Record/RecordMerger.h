#pragma once

#include <vector>
#include <map>

#include "RecordID.h"
#include "RecordMemory.h"
#include "IRecordSource.h"

struct RecordMerger
{
    static RecordMemory Merge(std::map<uint16_t, IRecordSource*>& sources, RecordID recordId);
};