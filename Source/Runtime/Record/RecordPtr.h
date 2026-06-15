#pragma once

#include "Record.h"

#include <GarbageCollector/ReferenceCounterPtr.h>

template <RecordClass RecordT = Record>
class WeakRecordPtr
{
    RecordID m_id = INVALID_RECORD;
    RecordT* m_record = nullptr;
public:
    WeakRecordPtr() {}
    WeakRecordPtr(RecordT* record) : m_record(record) {
        if(m_record) { 
            m_id = record->GetID();
        }
    }
    WeakRecordPtr(RecordID recordID) : m_id(recordID) {}
    WeakRecordPtr(RecordID recordID, RecordT* record) : m_id(recordID), m_record(record) { }

    RecordT* Get() { return m_record; }

    bool IsBound() const { return m_record != nullptr && !m_record->HasFlag(RecordFlags::Deleted); }
};

template <RecordClass RecordT = Record>
class RecordPtr : public ReferenceCounterPtr
{
protected:
    RecordID m_id = INVALID_RECORD;
    RecordT* m_record = nullptr;
public:
    // Creation constructors
    RecordPtr() {}
    RecordPtr(RecordT* record) : m_record(record) {
        if(m_record) { 
            m_id = m_record->GetID();
            ReferenceCounterPtr::AddRef(m_record);
        }
    }
    RecordPtr(RecordID recordID, RecordT* record) : m_id(recordID), m_record(record) { 
        if (m_record) ReferenceCounterPtr::AddRef(m_record);
    }

    // Copy constructor
    RecordPtr(const RecordPtr& other) : m_id(other.m_id), m_record(other.m_record) {
        if (m_record) ReferenceCounterPtr::AddRef(m_record);
    }

    // Copy assignment
    RecordPtr& operator=(const RecordPtr& other) {
        if (this != &other) {
            if (m_record) ReferenceCounterPtr::ReleaseRef(m_record);
            m_id = other.m_id;
            m_record = other.m_record;
            if (m_record) ReferenceCounterPtr::AddRef(m_record);
        }
        return *this;
    }

    // Move constructor
    RecordPtr(RecordPtr&& other) noexcept {
        m_id = other.m_id;
        m_record = other.m_record;
        other.m_record = nullptr;
        other.m_id = INVALID_RECORD;
    }
    
    // Move assignment
    RecordPtr& operator=(RecordPtr&& other) noexcept {
        if (this != &other) {
            if (m_record) ReferenceCounterPtr::ReleaseRef(m_record);
            m_id = other.m_id;
            m_record = other.m_record;
            other.m_id = INVALID_RECORD;
            other.m_record = nullptr;
        }
        return *this;
    }

    RecordT* operator ->() { 
        return m_record;
    }

    const RecordT* operator ->() const { 
        return m_record;
    }

    operator bool() const {return IsBound();}

    ~RecordPtr() {
        if(m_record) {
            ReferenceCounterPtr::ReleaseRef(m_record);
        }
    }

    RecordID GetID() const { return m_id; }

    RecordT* Get() const {
        return m_record;
    }

    void Release() {
        if(m_record) {
            ReferenceCounterPtr::ReleaseRef(m_record);
            m_record = nullptr;
        }
    }

    void Reset() {
        Release();
        m_id = 0;
    }

    WeakRecordPtr<RecordT> GetWeak() {
        return WeakRecordPtr<RecordT>(m_id, m_record);
    }

    // Checks if this record's instance is bound to the RecordPtr
    bool IsBound() const { return m_record != nullptr && !m_record->HasFlag(RecordFlags::Deleted); }
};

template <RecordClass CastTo, RecordClass CastFrom>
[[nodiscard]] inline RecordPtr<CastTo> CastRecord(const RecordPtr<CastFrom>& ptr) {
    if(!ptr.IsBound() || ptr->GetType() != CastTo::StaticType()) {
        return RecordPtr<CastTo>();
    }

    return RecordPtr<CastTo>(ptr.GetID(), static_cast<CastTo*>(ptr.Get()));
}