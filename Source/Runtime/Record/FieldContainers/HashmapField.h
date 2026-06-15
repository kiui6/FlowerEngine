#pragma once

#include "../FieldBase.h"

#include <unordered_map>

template <typename KeyT, FieldValueClass T>
struct FHashmap
{
    using DecayType = std::unordered_map<KeyT, typename T::DecayType>;

    static void Serialize(const DecayType& data, RecordFieldMemory& out) {}
    static void Deserialize(RecordFieldMemory* data, DecayType& out) {}
};