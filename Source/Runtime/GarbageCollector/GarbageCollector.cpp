#include "GarbageCollector.h"

#include <Config/ConfigManager.h>

#include <Log/Log.h>
#include <ranges>

bool GarbageCollector::bIsInitialized = RegisterService<GarbageCollector>({ConfigManager::GetStaticName()});

void GarbageCollector::Mark(ReferenceCounter *refCount)
{
    std::unique_lock queueLock(m_GCQueueMtx);
    m_unloadCandidates.insert(refCount);
}

void GarbageCollector::RunGCPass(bool unrestricted)
{
    if(!IsOwningThread()) {
        LOG(Assert, LogGC, "Starting GC pass outside of the owning Game Thread is prohibited");
        m_GCRequest.store(0x00, std::memory_order_release);
        return;
    }

    if(m_unloadCandidates.empty()) {
        LOG(Log, LogGC, "GC queue empty");
        m_GCRequest.store(0x00, std::memory_order_release);
        return;
    }

    LOG(Log, LogGC, "Starting GC pass");

    std::vector<ReferenceCounter*> unloadCandidates;

    // Lock queue mutex and move candidates to a local vector
    {
        std::unique_lock queueTransferLock(m_GCQueueMtx);
        unloadCandidates.reserve(m_unloadCandidates.size());
        std::ranges::copy(m_unloadCandidates, std::back_inserter(unloadCandidates));
        m_unloadCandidates.clear();
    }    

    const auto maxMicrosecondsInGC = std::chrono::microseconds(/*TODO: Load maxGCTime from config*/500);
    auto startTime = std::chrono::steady_clock::now();

    // Main GC loop
    while(unloadCandidates.size() > 0) {
        // Check if unload candidate is still eligible for unloading
        ReferenceCounter* ref = unloadCandidates.back();
        if(ref && ref->CanUnload()) {
            ref->RequestSelfDestruction();
        }

        unloadCandidates.pop_back();

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

    // Unlock queue mutex and move candidates back to original vector
    {
        std::unique_lock queueReturnLock(m_GCQueueMtx);
        m_unloadCandidates.insert_range(unloadCandidates);
    }    
    
    m_GCRequest.store(0x00, std::memory_order_release);
}

void GarbageCollector::RequestGCPass(bool unrestricted)
{
    uint8_t flags = unrestricted ? 0x03 : 0x01;

    m_GCRequest.store(flags, std::memory_order_release);
}
