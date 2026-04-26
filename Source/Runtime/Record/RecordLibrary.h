#pragma once

#include <Service/Service.h>

#include <Data/DataManager.h>
#include <Mixin/ThreadAware.h>

#include "Record.h"

#include <memory>
#include <atomic>
#include <shared_mutex>
#include <mutex>
#include <array>

class RecordLibrary : public IService, protected ThreadAware
{
    static bool bIsInitialized;

    mutable std::shared_mutex m_mtx;
    mutable std::mutex m_GCMtx;

    std::unordered_map<uint64_t, std::unique_ptr<Record>> m_records;
    
    // GC
    std::vector<RecordRefCount*> m_unloadCandidates;
    // bit 0x01 specifies whether request is posted
    // bit 0x02 specifies whether pass should be unrestricted
    std::atomic<uint8_t> m_GCRequest{0x00};
public:
    static std::string_view GetStaticName() {return "RecordLibrary";}

    virtual void Initialize() override;
    virtual void Deinitialize() override;

    // TODO: Create RecordPtr class
    Record* LoadRecord(uint64_t recordID);
    Record* GetRecord(uint64_t recordID);
    void UnloadRecord(uint64_t recordID);
    bool IsValidRecord(uint64_t recordID) const;

    // Marks Record as garbage, will cause deletion on the next GC pass, unless reference is reacquired
    // Will not unload record unless refCount is actually zero when GC pass runs
    void MarkForUnload(RecordRefCount* refCount);

    /* Forces immediate synchronous GC pass
     *
     * @param bool unrestricted
     * Removes time restrictions for this GC pass
     */
    void RunGCPass(bool unrestricted = false);

    /* Requests synchronous GC pass next frame
     *
     * @param bool unrestricted
     * Removes time restrictions for this GC pass
     */
    void RequestGCPass(bool unrestricted = false);

    inline bool IsGCPassRequested() const { return m_GCRequest.load(std::memory_order_acquire) & 0x01;}
    inline bool IsRequestedGCPassUnrestricted() const { return m_GCRequest.load(std::memory_order_acquire) & 0x02;}
};