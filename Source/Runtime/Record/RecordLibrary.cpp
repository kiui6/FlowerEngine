#include "RecordLibrary.h"

#include <chrono>

#include <Log/Log.h>
#include "RecordFactoryLibrary.h"

bool RecordLibrary::bIsInitialized = RegisterService<RecordLibrary>({DataManager::GetStaticName()});

void RecordLibrary::Initialize()
{
    m_datamgr = GetService<DataManager>();
}

void RecordLibrary::Deinitialize()
{
}

RecordPtr<Record> RecordLibrary::CreateRecordFromType(uint32_t recordType, uint8_t pluginID)
{
    std::unique_lock lock(m_mtx);
    RecordID id = GenerateRecordID();

    RecordFactory* factory = RecordFactoryLibrary::Get().GetFactory(recordType);
    if(!factory) {
        LOGF(Assert, LogRecord, "Factory of specified type %c%c%c%c does not exist. Did you register it?", recordType, recordType >> 8, recordType >> 16, recordType >> 24);
        return nullptr;
    }

    Record* record;
    record = factory->NewRecord();
    record->SetID(id);

    m_records.emplace(id, record);

    LOGF(Log, LogRecord, "Created Record[0x%016llX] of Type[%c%c%c%c]", id, recordType, recordType >> 8, recordType >> 16, recordType >> 24);

    return RecordPtr<Record>(id, record);
}

RecordPtr<Record> RecordLibrary::LoadRecordRaw(RecordID recordID)
{
    return RecordPtr<Record>();
}

RecordPtr<Record> RecordLibrary::LoadRecordOfType(RecordID recordID, ID32 type)
{
    return RecordPtr<Record>();
}

RecordPtr<Record> RecordLibrary::GetRecordRaw(RecordID recordID)
{
    return RecordPtr<Record>();
}

RecordPtr<Record> RecordLibrary::GetRecordOfType(RecordID recordID, ID32 type)
{
    return RecordPtr<Record>();
}

void RecordLibrary::UnloadRecord(RecordID recordID)
{
    std::unique_lock lock(m_mtx);
}

bool RecordLibrary::IsValidRecord(RecordID recordID) const
{
    std::shared_lock lock(m_mtx);
    return false;
}

RecordID RecordLibrary::GenerateRecordID()
{
    RecordID local = m_nextLocalID.fetch_add(1, std::memory_order_relaxed);
    return (static_cast<RecordID>(s_runtimeModID) << 56) | (local & 0x00FFFFFFFFFFFFFF);
}
