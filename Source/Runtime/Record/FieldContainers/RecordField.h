#pragma once

#include "../FieldBase.h"
#include "../RecordPtr.h"

// TODO: Make templated type check
struct RecordField
{
    using DecayType = RecordID;

    static DecayType DefaultConstructor() {return INVALID_RECORD;}

    static void Serialize(const DecayType& data, std::vector<uint8_t>& out){}
    static void Deserialize(std::span<const uint8_t> data, DecayType& out){}
};