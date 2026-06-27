#pragma once

#include "../FieldBase.h"

#include <Utility/Meta/IsTrivial.h>

#include <cstdint>

template <IsTrivial T>
struct FTrivial
{
    using DecayType = T;

    static void Serialize(const DecayType& data, RecordFieldObject::NodeWrapper& out) {}
    static void Deserialize(FieldNode* node, DecayType& out) {}
};

using FInt8 = FTrivial<int8_t>;
using FInt16 = FTrivial<int16_t>;
using FInt32 = FTrivial<int32_t>;
using FInt64 = FTrivial<int64_t>;

using FUInt8 = FTrivial<uint8_t>;
using FUInt16 = FTrivial<uint16_t>;
using FUInt32 = FTrivial<uint32_t>;
using FUInt64 = FTrivial<uint64_t>;

using FFloat = FTrivial<float>;
using FDouble = FTrivial<double>;