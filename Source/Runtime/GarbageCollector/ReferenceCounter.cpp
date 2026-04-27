#include "ReferenceCounter.h"

#include "GarbageCollector.h"

void ReferenceCounter::AddRef()
{
    m_refCount.fetch_add(1, std::memory_order_relaxed);
}

void ReferenceCounter::ReleaseRef()
{
    if (m_refCount.fetch_sub(1, std::memory_order_release) == 1) {
        std::atomic_thread_fence(std::memory_order_acquire);
        GetService<GarbageCollector>()->Mark(this);
    }
}
