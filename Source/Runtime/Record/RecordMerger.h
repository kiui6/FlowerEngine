#pragma once

#include <vector>
#include <map>
#include <optional>

#include "RecordID.h"
#include "RecordMemory.h"
#include "IRecordSource.h"

#include <Utility/Defines.h>
#include <Utility/Memory/Arena.h>

#include "RecordIR/RecordObject.h"

struct RecordMerger
{
    std::vector<RecordMemory> m_memories;
    GrowingArena<16*1024> m_NodeArena;
public:

    void ResizeMemoryPool(size_t newPoolSize) {
        if(newPoolSize > m_memories.size()) {
            m_memories.resize(newPoolSize);
        }
    }

    NODISCARD bool Merge(std::map<uint16_t, IRecordSource*>& sources, RecordID recordId, RecordMemory& result);
};