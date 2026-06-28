#include "RecordMerger.h"

#include <Log/Log.h>

uint8_t RecordMerger::LoadAndMerge(std::map<uint16_t, IRecordSource*>& sources, RecordID recordID, RecordObject*& resObject)
{
    std::shared_lock<std::shared_mutex> lock(m_objectsMutex);

    uint8_t objectPos = AcquireObject();
    if(objectPos == NULL_OBJECT) {
        return objectPos;
    }

    resObject = &m_objects[objectPos];
    resObject->SetID(recordID);

    // Passed RecordId has pluginID(most significant 2 bytes) relative to current load order
    // It means that it's only correct in this engine sessions's context
    // And its relative pluginID points to this session's plugin order-relative source
    // We extract unique pluginID from this source to search inside plugins relying on it as a master
    uint16_t recordRelativePluginID = ExtractPluginIDFromRecordID(recordID);
    if(sources.size() <= recordRelativePluginID) {
        LOG(Assert, LogRecordMerger, "Record doesn't exist: Plugin Order ID doesn't exist!");
        FreeObject(objectPos);
        return NULL_OBJECT;
    }

    RecordInformation recInfo;

    // We know where the base is located from RecordID's PluginOrderID
    // So we can load it right away and then scan for deltas after it
    RecordID baseRecordID = RemovePluginIDFromRecordID(recordID);
    if(!sources[recordRelativePluginID]->FetchRecordInfo(baseRecordID, recInfo)) {
        LOG(Assert, LogRecordMerger, "Record doesn't exist!");
        FreeObject(objectPos);
        return NULL_OBJECT;
    }

    resObject->SetType(recInfo.type);
    if(recInfo.isDeleted) {
        LOG(Warning, LogRecordMerger, "Record was deleted!");
        FreeObject(objectPos);
        return NULL_OBJECT;
    }

    sources[recordRelativePluginID]->PopulateRecordFieldObject(baseRecordID, *resObject);

    uint64_t recordUniquePluginID = sources[recordRelativePluginID]->GetUniquePluginID();
    bool hasDeltas = false;
    PluginOrderID sourceResolvedRelativeID;
    RecordID sourceResolvedRecordID;

    // Scan for deltas
    for(size_t i = recordRelativePluginID + 1; i < sources.size(); i++) {
        // Fetches its relative mapping to a uniquePluginID of a plugin holding this record
        // Fetches 0x0000 if record belongs to this plugin 
        // Returns false if this plugin has no relation to requested plugin(and its record)
        if(!sources[i]->ResolveUniquePluginID(recordUniquePluginID, sourceResolvedRelativeID)) {
            continue;
        }

        sourceResolvedRecordID = OverwritePluginIDInRecordID(recordID, sourceResolvedRelativeID);
        
        // Skip if this source doesn't have record we're searching for
        if(!sources[i]->FetchRecordInfo(sourceResolvedRecordID, recInfo)) {
            continue;
        }

        if(recInfo.isDeleted) {
            LOG(Warning, LogRecordMerger, "Record was deleted!");
            FreeObject(objectPos);
            return NULL_OBJECT;
        }

        // 0x0000 relative plugin ID is reserved only for plugin's own records(aka base records).
        // If we haven't found a base yet, we don't care about deltas. 
        // Even if we find a delta, it means load order is fatally incorrect and read can't procceed.
        if(sourceResolvedRelativeID == PLUGIN_ORDER_ID_BASE) {
            LOG(Assert, LogRecordMerger, "Found base, but expected delta!");
            FreeObject(objectPos);
            return NULL_OBJECT;
        }

        hasDeltas = true;
    }

    if(hasDeltas) {
        // Perform final merge
    } else {
        resObject->SetFinal(0);
    }

    return objectPos;
}