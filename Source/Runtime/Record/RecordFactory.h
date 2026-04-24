#pragma once

#include "Record.h"
#include <World/Actor.h>
#include <Utility/Record.h>

#include <memory>
#include <array>
#include <concepts>

struct RecordFactory
{
    virtual uint32_t GetRecordType() {return MakeRecordType('U','N','K','N');}
    virtual std::unique_ptr<Record> NewRecord() {return nullptr;}
    virtual std::shared_ptr<Actor> CreateWorldActorFromRecord() {return nullptr;}
    virtual bool ShowInRecordList() {return false;}
};

template <RecordClass RecordType>
struct TemplatedRecordFactory : public RecordFactory
{
    virtual uint32_t GetRecordType() override {return RecordType::GetStaticType();}
    virtual std::unique_ptr<Record> NewRecord() override {return std::make_unique<RecordType>();}
};