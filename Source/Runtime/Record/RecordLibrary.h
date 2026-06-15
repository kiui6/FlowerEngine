#pragma once

#include <Service/Service.h>

#include <Data/DataManager.h>
#include <Mixin/ThreadAware.h>
#include <Log/Log.h>

#include "Record.h"
#include "RecordPtr.h"
#include "IRecordSource.h"
#include "RecordMerger.h"

#include <memory>
#include <atomic>
#include <shared_mutex>
#include <mutex>
#include <array>
#include <map>

class RecordLibrary : public IService
{
    static bool bIsInitialized;
    static constexpr uint16_t s_runtimeModID = 0xFFFF;

    mutable std::shared_mutex m_mtx;

    std::map<uint16_t, IRecordSource*> m_sources;
    RecordMerger m_merger;

    DataManager* m_datamgr = nullptr;

    std::unordered_map<RecordID, std::unique_ptr<Record>> m_records;

    // If editing Master File, should be set outside of bounds of reserved IDs
    std::atomic<RecordID> m_nextLocalID{1};
public:
    RecordLibrary();

    static std::string_view GetStaticName() {return "RecordLibrary";}

    virtual void Initialize() override;
    virtual void Deinitialize() override;

    /*
     * Adds record source to a specific load order index. Should also be used to load record changes from save files
     * 
     * @note
     * Engine's IO access is sandboxed, so file path should be a sandboxed path ("Game/Plugin.plugin" or "Pref/Savefile0.save")
     * 
     * For loading save files, pluginID should be set to 0xFFFF
     * 
     * @param pluginID
     * Sets index in the load order of the mounted plugin. Use 0 for automatic indexing(ignores 0xFFFF)
     * 
     * @returns
     * false if pluginID is already taken
     */
    bool AddRecordSource(IRecordSource* source, uint16_t pluginID = 0);
    bool RemoveRecordSource(uint16_t pluginID = 0);

    /*
     * Creates new serializable record and puts it into library
     * Created record will be assigned pluginID of 0xFF (Runtime Records)
     * 
     * @note
     * In editor: Will be serialized into master or plugin file.
     * 
     * In game: Will be serialized into save file.
     * 
     * Before using result, caller should check `IsBound()` before accessing record.
     * 
     * @param pluginID
     * Used for debug purposes, automatically set to `s_runtimeModID`(0xFF). Should never be changed.
     * 
     * @returns
     * RecordPtr of requested type. returns unbound(RecordPtr::IsBound() == false) RecordPtr if creation failed. 
     */
    template <RecordClass T>
    [[nodiscard]] RecordPtr<T> CreateRecord(uint16_t pluginID = s_runtimeModID);

    /*
     * Creates new serializable record of specified type and puts it into library
     * Created record will be assigned pluginID of 0xFF (Runtime Records)
     * 
     * @note
     * In editor: Will be serialized into master or plugin file.
     * 
     * In game: Will be serialized into save file.
     * 
     * Before using result, caller should check `IsBound()` before accessing record.
     * 
     * @param recordType
     * ID32 type of record to create. ID32 type can be constructed using MakeID32("CODE") from <Utility/ID.h>
     * 
     * @param pluginID
     * Used for debug purposes, automatically set to `s_runtimeModID`(0xFF). Should never be changed.
     * 
     * @returns
     * RecordPtr of requested type. returns unbound(RecordPtr::IsBound() == false) RecordPtr if creation failed. 
     */
    [[nodiscard]] RecordPtr<Record> CreateRecordFromType(uint32_t recordType, uint16_t pluginID = s_runtimeModID);

    /*
     * Fetches already loaded or synchronously loads the record from data files using its ID
     * 
     * @note
     * Before using result, caller should check `IsBound()` before accessing record
     * 
     * @param recordID
     * ID of a record to load
     * 
     * @returns
     * RecordPtr of record with requested type. returns unbound(RecordPtr::IsBound() == false) RecordPtr if couldn't fetch or load record. 
     */
    template <RecordClass T>
    [[nodiscard]] RecordPtr<T> LoadRecord(RecordID recordID);

    /*
     * Fetches already loaded or synchronously loads the record from data files using its ID
     * 
     * @note
     * Before using result, caller should check `IsBound()` before accessing record
     * 
     * @param recordID
     * ID of a record to load
     * 
     * @returns
     * RecordPtr of polymorphic record. returns unbound(RecordPtr::IsBound() == false) RecordPtr if couldn't fetch or load record. 
     */
    [[nodiscard]] RecordPtr<Record> LoadRecordRaw(RecordID recordID);

    /*
     * Fetches already loaded or synchronously loads the record from data files using its ID
     * Compares type of record against the requested type before returning
     * 
     * @note
     * Before using result, caller should check `IsBound()` before accessing record
     * 
     * @param recordID
     * ID of a record to load
     * 
     * @param type
     * ID32 type of record to create. ID32 type can be constructed using MakeID32("CODE") from <Utility/ID.h>
     * 
     * @returns
     * RecordPtr of record with requested type. returns unbound(RecordPtr::IsBound() == false) RecordPtr if couldn't fetch or load record, 
     * or record's type doesn't match requested type. 
     */
    [[nodiscard]] RecordPtr<Record> LoadRecordOfType(RecordID recordID, ID32 type);

    template <RecordClass T>
    [[nodiscard]] RecordPtr<T> GetRecord(RecordID recordID);
    [[nodiscard]] RecordPtr<Record> GetRecordRaw(RecordID recordID);
    [[nodiscard]] RecordPtr<Record> GetRecordOfType(RecordID recordID, ID32 type);
    
    void UnloadRecord(RecordID recordID);
    bool IsValidRecord(RecordID recordID) const;

    void DeleteRecord(RecordID recordID);
    
    /*
     *  Returns caller the unique incremental ID for runtime use (with 0xFF prefix)
     *  This reserved ID can never be claimed again in runtime.
     *  Doesn't require record associated with it.
     */
    RecordID ReserveLocalRecordID() {return GenerateRecordID(); }
protected:
    RecordID GenerateRecordID();
    inline std::unique_ptr<Record> CreateEmptyRecordFromType(ID32 type);
};

template <RecordClass T>
inline RecordPtr<T> RecordLibrary::CreateRecord(uint16_t pluginID)
{
    std::unique_lock lock(m_mtx);

    RecordID id = GenerateRecordID();
    T* record = new T();
    record->SetID(id);

    m_records.emplace(id, std::unique_ptr<Record>(record));

    if constexpr(IS_VERBOSE) {
        LOGF(Log, LogRecord, "Created Record[0x%016llX] of Type[%c%c%c%c]", id, T::StaticType(), T::StaticType() >> 8, T::StaticType() >> 16, T::StaticType() >> 24);
    }

    return RecordPtr<T>(id, record);
}

template <RecordClass T>
inline RecordPtr<T> RecordLibrary::LoadRecord(RecordID recordID)
{
    std::unique_lock lock(m_mtx);

    if(recordID == INVALID_RECORD) {
        LOGF(Error, LogRecord, "Attempted loading INVALID_RECORD of Type[%c%c%c%c]", T::StaticType(), T::StaticType() >> 8, T::StaticType() >> 16, T::StaticType() >> 24);
        return {INVALID_RECORD, nullptr};
    }

    // Try find record by ID
    auto it = m_records.find(recordID);
    if(it != m_records.end()) {
        if(it->second->GetType() == T::StaticType()) {
            return {recordID, static_cast<T*>(it->second.get())};
        }
    }

    // TODO: Implement merging
    RecordMemory memory;
    if(!m_merger.Merge(m_sources, recordID, memory)) {
        LOGF(Error, LogRecord, "Failed to load Record[0x%016llX] of Type[%c%c%c%c]", recordID, T::StaticType(), T::StaticType() >> 8, T::StaticType() >> 16, T::StaticType() >> 24);
        return {};
    }

    if(memory.IsDeleted()) {
        LOGF(Error, LogRecord, "Failed to load Record[0x%016llX] of Type[%c%c%c%c]. Record is deleted", recordID, T::StaticType(), T::StaticType() >> 8, T::StaticType() >> 16, T::StaticType() >> 24);
            return {};
    }

    if(memory.GetType() != T::StaticType()) {
        LOGF(Error, LogRecord, "Failed to load Record[0x%016llX] of Type[%c%c%c%c]. Record type mismatch, actual type is Type[%c%c%c%c]", recordID, 
            T::StaticType(), T::StaticType() >> 8, T::StaticType() >> 16, T::StaticType() >> 24,
            memory.GetType(), memory.GetType() >> 8, memory.GetType() >> 16, memory.GetType() >> 24);
            return {};
    }

    T* record = new T();
    record->SetID(recordID);

    for(FieldBase* field : record->GetFields()) {
        RecordFieldMemory* fieldMem = memory.GetField(field->GetID());
        if(!fieldMem) {
            /*
            LOGF(Warning, LogRecord, "Found no Field[%c%c%c%c] data for Record[0x%016llX] of Type[%c%c%c%c].", 
                field->GetID(), field->GetID() >> 8, field->GetID() >> 16, field->GetID() >> 24,
                recordID, 
                T::StaticType(), T::StaticType() >> 8, T::StaticType() >> 16, T::StaticType() >> 24);
            */
            continue;
        }
        field->Deserialize(fieldMem);
    }

    m_records.emplace(recordID, std::unique_ptr<Record>(record));

    if constexpr(IS_VERBOSE) {
        LOGF(Log, LogRecord, "Loaded Record[0x%016llX] of Type[%c%c%c%c]", recordID, T::StaticType(), T::StaticType() >> 8, T::StaticType() >> 16, T::StaticType() >> 24);
    }

    return {recordID, record};
}

template <RecordClass T>
inline RecordPtr<T> RecordLibrary::GetRecord(RecordID recordID)
{
    std::shared_lock lock(m_mtx);
    
    auto it = m_records.find(recordID);
    if(it != m_records.end()) {
        if(it->second->GetType() == T::StaticType()) {
            return {recordID, static_cast<T*>(it->second.get())};
        }
    }
    
    return {recordID, nullptr};
}