#pragma once

#include <mutex>
#include <vector>
#include <atomic>
#include <cstdint>
#include <unordered_set>

#include <Service/Service.h>
#include <Mixin/ThreadAware.h>

#include "ReferenceCounter.h"

class GarbageCollector : public IService, private ThreadAware
{
    static bool bIsInitialized;

    // GC Queue is separate from GCPass, when it's copied and freed, so that other objects can mark themselves.
    mutable std::mutex m_GCQueueMtx;

    // GC
    std::unordered_set<ReferenceCounter*> m_unloadCandidates;
    // bit 0x01 specifies whether request is posted
    // bit 0x02 specifies whether pass should be unrestricted
    std::atomic<uint8_t> m_GCRequest{0x00};
public:
    // Marks object as garbage, will cause deletion on the next GC pass, unless reference is reacquired
    // Will not unload object unless refCount is actually zero when GC pass runs
    void Mark(ReferenceCounter* refCount);

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