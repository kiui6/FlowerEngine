#pragma once

#include "Record.h"
#include <World/Actor.h>
#include <Utility/Record.h>

#include <memory>
#include <array>
#include <concepts>

struct RecordFactory
{
    virtual constexpr uint32_t GetRecordType() {return CompMakeRecordType("UNKN");}
    virtual std::unique_ptr<Record> NewRecord() {return nullptr;}
    virtual std::shared_ptr<Actor> CreateWorldActorFromRecord() {return nullptr;}
    virtual constexpr bool ShowInRecordList() {return false;}
};

template <RecordClass RecordType>
struct TemplatedRecordFactory : public RecordFactory
{
    virtual constexpr uint32_t GetRecordType() override {return RecordType::StaticType();}
    virtual std::unique_ptr<Record> NewRecord() override {return std::make_unique<RecordType>();}
};