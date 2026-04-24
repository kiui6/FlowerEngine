#include "RecordRefCount.h"

#include "RecordLibrary.h"

void RecordRefCount::AddRef()
{
    std::lock_guard guard(m_refMtx);
    m_refCount.fetch_add(1, std::memory_order_relaxed);
}

void RecordRefCount::ReleaseRef()
{
    std::lock_guard guard(m_refMtx);
    if (m_refCount.fetch_sub(1, std::memory_order_release) == 1) {
        std::atomic_thread_fence(std::memory_order_acquire);
        GetService<RecordLibrary>()->MarkForUnload(this);
    }
}