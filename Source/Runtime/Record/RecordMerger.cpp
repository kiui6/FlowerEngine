#include "RecordMerger.h"

#include <Log/Log.h>

uint8_t RecordMerger::LoadAndMerge(std::map<uint16_t, IRecordSource*>& sources, RecordID recordId, RecordObject*& resObject)
{
    std::shared_lock<std::shared_mutex> lock(m_objectsMutex);

    uint8_t objectPos = AcquireObject();
    RecordObject& object = m_objects[objectPos];
    for(const auto& [key, source] : sources) {
        source->FetchRecordObject(recordId, object);
        return objectPos;
    }

    return objectPos;

    // TODO
    if(sources.size() == 0) {
        LOG(Warning, LogRecordMerger, "No sources provided for merging");
        return false;
    }

    ResizeMemoryPool(sources.size());
    for(size_t sourceIndex = 0; sourceIndex < sources.size(); ++sourceIndex) {
        IRecordSource* source = sources[sourceIndex];
        
        RecordObject& memoryForSource = m_objects[sourceIndex];
        memoryForSource.Reset();
        if(!source->FetchRecordObject(recordId, memoryForSource)) {
            continue;
        }
    }

    return 0xFF;
}