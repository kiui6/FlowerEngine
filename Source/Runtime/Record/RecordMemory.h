#pragma once

#include "RecordFieldMemory.h"

#include <Utility/ID.h>

#include <unordered_map>
#include <optional>

class RecordMemory {
    friend class RecordMerger;

    std::unordered_map<ID32, RecordFieldMemory> m_fieldMems;
    bool m_isDeleted = false;
public:
    std::optional<RecordFieldMemory*> GetField(ID32 id) {
        const auto& pair = m_fieldMems.find(id);
        if(pair != m_fieldMems.end()) {
            return &pair->second;
        }

        return {};
    }

    inline bool IsDeleted() const {return m_isDeleted;}
};