#include "RecordMerger.h"

#include <Log/Log.h>

bool RecordMerger::Merge(std::map<uint16_t, IRecordSource*>& sources, RecordID recordId, RecordMemory& result)
{
    for(const auto& [key, source] : sources) {
        return source->FetchRecordMemory(recordId, result);
    }

    return false;

    // TODO
    if(sources.size() == 0) {
        LOG(Warning, LogRecordMerger, "No sources provided for merging");
        return false;
    }

    ResizeMemoryPool(sources.size());
    for(size_t sourceIndex = 0; sourceIndex < sources.size(); ++sourceIndex) {
        IRecordSource* source = sources[sourceIndex];
        
        RecordMemory& memoryForSource = m_memories[sourceIndex];
        memoryForSource.Reset();
        if(!source->FetchRecordMemory(recordId, memoryForSource)) {
            continue;
        }
    }

    return true;
}