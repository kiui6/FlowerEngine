#pragma once

#include <vector>
#include <map>
#include <optional>
#include <mutex>
#include <shared_mutex>
#include <atomic>

#include "RecordID.h"
#include "IRecordSource.h"

#include <Utility/Defines.h>

#include "RecordIR/RecordObject.h"

/*
 * Central class for reading, merging and writing record objects with fields
 *
 * Supports concurrency
 */
struct RecordMerger
{
public:
    static constexpr const uint8_t NULL_OBJECT = 0xff;
private:
    std::shared_mutex m_objectsMutex{};

    std::vector<RecordObject> m_objects{};

    std::atomic_uint16_t m_poolUsedBits{0};

    inline uint8_t AcquireObject() {
        uint16_t mask = m_poolUsedBits.load(std::memory_order_acquire);
        while(true) {
            if(mask == 0xFFFF) {
                return NULL_OBJECT;
            }

            uint16_t freeMask = ~mask;
            uint16_t lowestFreeBit = freeMask & -freeMask;

            uint16_t newMask = mask | lowestFreeBit;
            if (m_poolUsedBits.compare_exchange_weak(mask, newMask, 
                                                     std::memory_order_release,
                                                     std::memory_order_acquire)) {
                return static_cast<uint8_t>(std::countr_zero(lowestFreeBit));
            }
        }
    }
public:
    inline void ResizeMemoryPool(uint8_t newPoolSize) {
        std::unique_lock<std::shared_mutex> lock(m_objectsMutex);
        if(newPoolSize >= 16) {
            m_objects.resize(16);
            m_poolUsedBits.store(0, std::memory_order_release);
        } else {
            m_objects.resize(newPoolSize < 16 ? newPoolSize : 16);
            m_poolUsedBits.store(~((1u << newPoolSize) - 1u), std::memory_order_release);
        }
    }

    NODISCARD uint8_t LoadAndMerge(std::map<uint16_t, IRecordSource*>& sources, RecordID recordId, RecordObject*& resObject);

    inline void FreeObject(uint8_t pos) {
        if(pos > 16) return;

        m_objects[pos].Reset();

        uint16_t posBit = 1u << pos;
        m_poolUsedBits.fetch_and(~posBit, std::memory_order_release);
    }
};