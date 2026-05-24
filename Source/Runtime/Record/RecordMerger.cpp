#include "RecordMerger.h"

bool RecordMerger::Merge(std::map<uint16_t, IRecordSource*>& sources, RecordID recordId, RecordMemory& result)
{
    for(const auto& [key, source] : sources) {
        source->FetchRecordMemory(recordId, result);
    }

    return true;
}