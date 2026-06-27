#include "RecordMerger.h"

#include <Log/Log.h>

uint8_t RecordMerger::LoadAndMerge(std::map<uint16_t, IRecordSource*>& sources, RecordID recordId, RecordObject*& resObject)
{
    std::shared_lock<std::shared_mutex> lock(m_objectsMutex);

    // Passed RecordId has pluginID(most significant 2 bytes) relative to current load order
    // It means that it's only correct in this engine sessions's context
    // And its relative pluginID points to this session's plugin order-relative source
    // We extract unique pluginID from this source to search inside plugins relying on it as a master
    uint16_t recordRelativePluginID = static_cast<uint16_t>(recordId >> 48);
    if(sources.size() <= recordRelativePluginID) {
        return NULL_OBJECT;
    }

    uint64_t recordUniquePluginID = sources[recordRelativePluginID]->GetUniquePluginID();

    uint8_t objectPos = AcquireObject();
    if(objectPos == NULL_OBJECT) {
        return objectPos;
    }

    bool baseFound = false;
    bool hasDeltas = false;

    // Temporary information we don't want to recreate every pass
    RecordInformation recInfo;
    uint16_t sourceResolvedRelativeID;
    uint64_t sourceResolvedRecordID;

    resObject = &m_objects[objectPos];
    resObject->SetID(recordId);

    for(const auto& [key, source] : sources) {
        // Fetches its relative mapping to a uniquePluginID of a plugin holding this record
        // Fetches 0x0000 if record belongs to this plugin 
        // Returns false if this plugin has no relation to requested plugin(and its record)
        if(!source->ResolveUniquePluginID(recordUniquePluginID, sourceResolvedRelativeID)) {
            continue;
        }
        
        // 0x0000 relative plugin ID is reserved only for this plugin's own records(aka base records).
        // If we haven't found a base yet, we don't care about deltas. 
        // Even if we find a delta, it means load order is fatally incorrect and read can't procceed.
        // TODO: Decide what to do. Maybe provide startup-time guarantee that no wrong order can ever happen?
        if(sourceResolvedRelativeID != 0x0000 & !baseFound) {
            LOG(Assert, LogRecordMerger, "Found delta before base when loading record!");
            FreeObject(objectPos);
            return NULL_OBJECT;
        }

        sourceResolvedRecordID = recordId & ~(0xFFFFULL << 48);
        sourceResolvedRecordID |= (static_cast<uint64_t>(sourceResolvedRelativeID) << 48);
        
        if(!baseFound) { 
            if(!source->FetchRecordInfo(sourceResolvedRecordID, recInfo)) {
                continue;
            }

            resObject->SetType(recInfo.type);
            resObject->SetDeleted(recInfo.isDeleted);
            if(recInfo.isDelta)
            resObject->SetDeleted(recInfo.isDeleted);
        }

        source->PopulateRecordFieldObject(recordId, *resObject);
        return objectPos;
    }

    return objectPos;
}