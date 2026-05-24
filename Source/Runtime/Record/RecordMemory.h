#pragma once

#include "RecordFieldMemory.h"

#include <Utility/ID.h>

#include <unordered_map>
#include <optional>

class RecordMemory {
    friend class RecordMerger;

    std::unordered_map<ID32, RecordFieldMemory> m_fieldMems;
    uint64_t m_recordID = 0;
    ID32 m_type = 0;
    bool m_isDeleted = false;
    bool m_isOverride = false;
public:
    inline RecordFieldMemory& AddFieldAndRetrieveMemory(ID32 fieldId) { 
        return m_fieldMems.emplace(fieldId, RecordFieldMemory{}).first->second;
    }
    inline void SetRecordID(uint64_t id) {m_recordID = id;}
    inline void SetRecordType(ID32 type) {m_type = type;}
    inline void SetDeleted(bool val) {m_isDeleted = val;}
    inline void SetOverride(bool val) {m_isOverride = val;}

    inline ID32 GetID() const {return m_recordID;}
    inline ID32 GetType() const {return m_type;}
    inline bool IsDeleted() const {return m_isDeleted;}
    
    inline RecordFieldMemory* GetField(ID32 id) {
        const auto& pair = m_fieldMems.find(id);
        if(pair != m_fieldMems.end()) {
            return &pair->second;
        }

        return {};
    }

};