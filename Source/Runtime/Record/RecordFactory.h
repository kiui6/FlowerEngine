#pragma once

#include "Record.h"
#include <Utility/Record.h>

#include <memory>
#include <array>
#include <concepts>

class Actor;

struct RecordFactory
{
    virtual constexpr uint32_t GetRecordType() {return CompMakeRecordType("UNKN");}
    virtual Record* NewRecord() {return nullptr;}
    /*
     * Creates an actor.
     */
    virtual Actor* CreateActor(Record* reference) {return nullptr;}
    virtual constexpr bool ShowInRecordList() {return false;}
};

template <RecordClass RecordType>
struct TemplatedRecordFactory : public RecordFactory
{
    virtual constexpr uint32_t GetRecordType() override {return RecordType::StaticType();}
    virtual Record* NewRecord() override {
        Record* record = new RecordType();
        record->SetType(GetRecordType());
        return record;
    }
};