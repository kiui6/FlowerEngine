#pragma once

#include "Record.h"
#include <Utility/ID.h>

#include <memory>
#include <array>
#include <concepts>

class Actor;

struct RecordFactory
{
    virtual constexpr ID32 GetRecordType() {return MakeID32("UNKN");}
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
    virtual constexpr ID32 GetRecordType() override {return RecordType::StaticType();}
    virtual Record* NewRecord() override {
        Record* record = new RecordType();
        record->SetType(GetRecordType());
        return record;
    }
};