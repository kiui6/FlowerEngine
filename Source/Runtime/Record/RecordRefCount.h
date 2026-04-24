#pragma once

#include <atomic>
#include <mutex>

class RecordRefCount
{
    std::atomic<uint32_t> m_refCount{0};
    std::mutex m_refMtx;

    void AddRef();
    void ReleaseRef();

    bool CanUnload() const {return m_refCount.load(std::memory_order_acquire) == 0;}

public:
    virtual ~RecordRefCount() = default;
private:
    friend class RecordPtrBase;
    friend class RecordLibrary;
};