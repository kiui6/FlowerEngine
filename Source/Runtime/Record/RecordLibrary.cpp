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

    return RecordPtr<Record>(id, record);
}

RecordPtr<Record> RecordLibrary::LoadRecord(uint64_t recordID)
{
    std::unique_lock lock(m_mtx);
    return nullptr;
}

RecordPtr<Record> RecordLibrary::GetRecord(uint64_t recordID)
{
    std::shared_lock lock(m_mtx);
    return nullptr;
}

void RecordLibrary::UnloadRecord(uint64_t recordID)
{
    std::unique_lock lock(m_mtx);
}

bool RecordLibrary::IsValidRecord(uint64_t recordID) const
{
    std::shared_lock lock(m_mtx);
    return false;
}

uint64_t RecordLibrary::GenerateRecordID(uint8_t pluginID)
{
    uint64_t local = m_nextLocalID.fetch_add(1, std::memory_order_relaxed);
    return (static_cast<uint64_t>(pluginID) << 56) | (local & 0x00FFFFFFFFFFFFFF);
}
