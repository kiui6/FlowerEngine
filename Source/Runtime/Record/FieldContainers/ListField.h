#pragma once

#include "../FieldBase.h"

#include <vector>

template <FieldValueClass T>
struct FList
{
    using DecayType = std::vector<typename T::DecayType>;

    static void Serialize(const DecayType& data, RecordFieldMemory& out) {}
    static void Deserialize(RecordFieldMemory* data, DecayType& out) {}
};