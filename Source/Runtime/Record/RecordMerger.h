#pragma once

#include <vector>
#include <map>
#include <optional>

#include "RecordID.h"
#include "RecordMemory.h"
#include "IRecordSource.h"

struct RecordMerger
{
    static bool Merge(std::map<uint16_t, IRecordSource*>& sources, RecordID recordId, RecordMemory& result);
};