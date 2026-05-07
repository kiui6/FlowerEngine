#pragma once

#include <Service/Service.h>

#include <Data/DataManager.h>
#include <Mixin/ThreadAware.h>
#include <Log/Log.h>

#include "Record.h"
#include "RecordPtr.h"

#include <memory>
#include <atomic>
#include <shared_mutex>
#include <mutex>
#include <array>

class RecordLibrary : public IService
{
    static bool bIsInitialized;
    static uint8_t s_runtimeModID;

    mutable std::shared_mutex m_mtx;

    std::unordered_map<RecordID, std::unique_ptr<Record>> m_records;

    std::atomic<RecordID> m_nextLocalID{1};
public:
    static std::string_view GetStaticName() {return "RecordLibrary";}

    virtual void Initialize() override;
    virtual void Deinitialize() override;

    /*
     * Creates new serializable record and puts it into library
     * Creates record will be assigned pluginID of 0xFF (Runtime Records)
     * 
     * In editor: Will be serialized into master or plugin file
     * In game: Will be serialized into save file
     */
    template <RecordClass T>
    RecordPtr<T> CreateRecord(uint8_t pluginID = s_runtimeModID);

    RecordPtr<Record> CreateRecordFromType(uint32_t recordType, uint8_t pluginID = s_runtimeModID);

    Record* LoadRecordRaw(RecordID recordID);
    RecordPtr<Record> LoadRecord(RecordID recordID);
    RecordPtr<Record> GetRecord(RecordID recordID);
    void UnloadRecord(RecordID recordID);
    bool IsValidRecord(RecordID recordID) const;

    RecordID ReserveLocalRecordID() {return GenerateRecordID(s_runtimeModID); }
protected:
    RecordID GenerateRecordID(uint8_t pluginID);
};

template <RecordClass T>
inline RecordPtr<T> RecordLibrary::CreateRecord(uint8_t pluginID)
{
    std::unique_lock lock(m_mtx);

    RecordID id = GenerateRecordID(pluginID);
    T* record = new T();
    record->SetID(id);
    record->SetType(T::StaticType());

    RecordPtr<T> recPtr(id, record);

    m_records.emplace(id, std::unique_ptr<Record>(record));

    LOGF(Log, LogRecord, "Created Record[0x%016llX] of Type[%c%c%c%c]", id, T::StaticType(), T::StaticType() >> 8, T::StaticType() >> 16, T::StaticType() >> 24);

    return recPtr;
}
