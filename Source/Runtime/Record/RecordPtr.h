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
public:
    virtual ~RecordPtrBase(){}
};

template <RecordClass RecordT = Record>
class RecordPtr : public RecordPtrBase
{
protected:
    RecordID m_id = INVALID_RECORD;
    RecordT* m_record = nullptr;
public:
    // Creation constructors
    RecordPtr() {}
    RecordPtr(RecordT* record) : m_record(record) {
        if(record) { 
            m_id = record->GetID();
            RecordPtrBase::AddRef(m_record);
        }
    }
    RecordPtr(RecordID recordID, bool load = false) : m_id(recordID) {
        if(load){
            m_record = Load();
        }
    }

    // Copy constructor
    RecordPtr(const RecordPtr& other) : m_record(other.m_record) {
        if (m_record) RecordPtrBase::AddRef(m_record);
    }

    // Copy assignment
    RecordPtr& operator=(const RecordPtr& other) {
        if (this != &other) {
            if (m_record) RecordPtrBase::ReleaseRef(m_record);
            m_id = other.m_id;
            m_record = other.m_record;
            if (m_record) RecordPtrBase::AddRef(m_record);
        }
        return *this;
    }

    // Move constructor
    RecordPtr(RecordPtr&& other) noexcept : m_id(other.m_id), m_record(other.m_record) {
        other.m_record = m_record;
        other.m_id = INVALID_RECORD;
    }
    
    // Move assignment
    RecordPtr& operator=(RecordPtr&& other) noexcept {
        if (this != &other) {
            if (m_record) RecordPtrBase::ReleaseRef(m_record);
            m_id = other.m_id;
            m_record = other.m_record;
            other.m_id = INVALID_RECORD;
            other.m_record = nullptr;
        }
        return *this;
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

template <RecordClass RecordT = Record>
class WeakRecordPtr
{
    RecordID m_id = INVALID_RECORD;
    RecordT* m_record = nullptr;
public:
    WeakRecordPtr() {}
    WeakRecordPtr(RecordT* record) : m_record(record) {
        if(record) { 
            m_id = record->GetID();
        }
    }
    WeakRecordPtr(RecordID recordID) : m_id(recordID) {}

    RecordT* Get() {
        if(m_record) {
            return m_record;
        }

        // TODO Fetchinf from RecordLibrary
        return m_record;
    }

    bool IsLoaded() const { return m_record != nullptr; }
    bool IsValid() const;
};