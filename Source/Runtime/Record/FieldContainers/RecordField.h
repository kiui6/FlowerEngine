#pragma once

#include "../FieldBase.h"
#include "../RecordPtr.h"

template <RecordClass T = Record>
struct RecordField
{
    using DecayType = RecordPtr<T>;

    static void Serialize(const DecayType& data, std::vector<uint8_t>& out);
    static void Deserialize(std::span<const uint8_t> data, DecayType& out);
};

template <RecordClass T = Record>
struct WeakRecordField
{
    using DecayType = WeakRecordPtr<T>;

    static void Serialize(const DecayType& data, std::vector<uint8_t>& out);
    static void Deserialize(std::span<const uint8_t> data, DecayType& out);
};