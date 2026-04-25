#pragma once

#include "../FieldBase.h"

#include <vector>

template <FieldValueClass T>
class ListField
{
    using DecayType = std::vector<T::DecayType>;

    static void Serialize(const DecayType& data, std::vector<uint8_t>& out);
    static void Deserialize(std::span<const uint8_t> data, DecayType& out);
};