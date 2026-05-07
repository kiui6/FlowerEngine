#include "RecordLibrary.h"

#include <chrono>

#include <Log/Log.h>
#include "RecordFactoryLibrary.h"

bool RecordLibrary::bIsInitialized = RegisterService<RecordLibrary>({DataManager::GetStaticName()});

uint8_t RecordLibrary::s_runtimeModID = 0xFF;

void RecordLibrary::Initialize()
{
    std::string_view data = GetService<DataManager>()->GetName();
}

void RecordLibrary::Deinitialize()
{
}

RecordPtr<Record> RecordLibrary::CreateRecordFromType(uint32_t recordType, uint8_t pluginID)
{
    std::unique_lock lock(m_mtx);
    RecordID id = GenerateRecordID(pluginID);

    RecordFactory* factory = RecordFactoryLibrary::Get().GetFactory(recordType);
    if(!factory) {
        LOGF(Assert, LogRecord, "Factory of specified type %c%c%c%c does not exist. Did you register it?", recordType, recordType >> 8, recordType >> 16, recordType >> 24);
        return nullptr;
    }

    Record* record;
    record = factory->NewRecord();
    record->SetType(recordType);
    record->SetID(id);

    m_records.emplace(id, record);

    return RecordPtr<Record>(id, record);
}

Record *RecordLibrary::LoadRecordRaw(RecordID recordID)
{
    std::unique_lock lock(m_mtx);

    // Try find record by ID
    auto it = m_records.find(recordID);
    if(it != m_records.end()) {
        return it->second.get();
    }

    // TODO: Attempt loading from merged data files

    return nullptr;
}

RecordPtr<Record> RecordLibrary::LoadRecord(RecordID recordID)
{
    return RecordPtr<Record>(recordID, LoadRecordRaw(recordID));
}

RecordPtr<Record> RecordLibrary::GetRecord(RecordID recordID)
{
    std::shared_lock lock(m_mtx);
    return nullptr;
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

RecordID RecordLibrary::GenerateRecordID(uint8_t pluginID)
{
    RecordID local = m_nextLocalID.fetch_add(1, std::memory_order_relaxed);
    return (static_cast<RecordID>(pluginID) << 56) | (local & 0x00FFFFFFFFFFFFFF);
}
