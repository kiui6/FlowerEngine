#pragma once

#include "Record.h"

class RecordPtrBase {
protected:
    void AddRef(RecordRefCount* record) {
        record->AddRef();
    }
    void ReleaseRef(RecordRefCount* record) {
        record->ReleaseRef();
    }
};

template <RecordClass RecordT>
class RecordPtr : public RecordPtrBase
{
    RecordID m_id = INVALID_RECORD;
    RecordT* m_record = nullptr;
public:
    RecordPtr() {}
    RecordPtr(RecordT* record) : m_record(record) {
        if(record) { 
            m_id = record->GetID();
        }
    }
    RecordPtr(RecordID recordID, bool load = false) : m_id(recordID) {
        if(load){
            m_record = Load();
        }
    }

    ~RecordPtr() {
        if(m_record) {
            RecordPtrBase::ReleaseRef(m_record);
        }
    }

    RecordT* Load() {
        if(m_record) {
            return m_record;
        }

        // TODO Loading from RecordPtr
        return m_record;
    }

    bool IsLoaded() const { return m_record != nullptr; }
    bool IsValid() const;
};