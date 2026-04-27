#pragma once

#include <atomic>
#include <mutex>

class ReferenceCounter
{
    std::atomic<uint32_t> m_refCount{0};

    void AddRef();
    void ReleaseRef();

    bool CanUnload() const {return m_refCount.load(std::memory_order_acquire) == 0;}

    virtual void RequestSelfDestruction() = 0;

public:
    virtual ~ReferenceCounter() = default;
private:
    friend class ReferenceCounterPtr;
    friend class GarbageCollector;
};