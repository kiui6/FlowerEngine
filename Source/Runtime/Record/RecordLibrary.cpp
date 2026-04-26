#include "RecordLibrary.h"

#include <chrono>

#include <Log/Log.h>

bool RecordLibrary::bIsInitialized = RegisterService<RecordLibrary>({DataManager::GetStaticName()});

void RecordLibrary::Initialize()
{
    std::string_view data = GetService<DataManager>()->GetName();
}

void RecordLibrary::Deinitialize()
{
}

Record *RecordLibrary::LoadRecord(uint64_t recordID)
{
    std::unique_lock lock(m_mtx);
    return nullptr;
}

Record *RecordLibrary::GetRecord(uint64_t recordID)
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

void RecordLibrary::MarkForUnload(RecordRefCount *refCount)
{
    std::unique_lock lock(m_GCMtx);
    m_unloadCandidates.push_back(refCount);
}

void RecordLibrary::RunGCPass(bool unrestricted)
{
    if(!IsOwningThread()) {
        LOG(Error, LogGC, "Starting GC pass outside of the owning Game Thread is prohibited");
        return;
    }

    std::unique_lock gcLock(m_GCMtx);

    LOG(Log, LogGC, "Starting GC pass");

    const auto maxMicrosecondsInGC = std::chrono::microseconds(/*TODO: Load maxGCTime from config*/500);
    auto startTime = std::chrono::steady_clock::now();

    // Main GC loop
    while(m_unloadCandidates.size() > 0) {
        {
            // Lock mutex to temporarily prevent other threads from performing any record operations
            std::unique_lock unloadLock(m_mtx);

            // Check if unload candidate is still eligible for unloading
            RecordRefCount* recordRef = m_unloadCandidates.back();
            if(recordRef && recordRef->CanUnload()) {
                Record* record = dynamic_cast<Record*>(recordRef);
                m_records[record->GetID()].reset(); 
            }

            m_unloadCandidates.pop_back();
        }

        // Update time in GC if restricted
        if(!unrestricted) {
            auto elapsedTime = std::chrono::steady_clock::now() - startTime;
            if (elapsedTime >= maxMicrosecondsInGC) {
                // Break loop if time budget is exhausted
                LOGF(Log, LogGC, "GC pass ended in %lldus", std::chrono::duration_cast<std::chrono::microseconds>(elapsedTime).count());
                break; 
            }
        }
    }

    LOG(Log, LogGC, "Ending GC pass");
}

void RecordLibrary::RequestGCPass(bool unrestricted)
{
    uint8_t flags = unrestricted ? 0x03 : 0x01;

    m_GCRequest.store(flags, std::memory_order_release);
}
