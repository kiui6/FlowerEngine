#pragma once

#include "Record.h"
#include <Utility/ID.h>

#include <memory>
#include <array>
#include <concepts>

#include <World/Actor/Actor.h>

struct RecordFactory
{
    virtual constexpr ID32 GetRecordType() {return MakeID32("UNKN");}
    virtual std::unique_ptr<Record> NewRecord() {return nullptr;}
    virtual constexpr bool ShowInRecordList() {return false;}
};

template <RecordClass RecordType>
struct TemplatedRecordFactory : public RecordFactory
{
    virtual constexpr ID32 GetRecordType() override {return RecordType::StaticType();}
    virtual std::unique_ptr<Record> NewRecord() override {
        return std::make_unique<RecordType>();
    }
};