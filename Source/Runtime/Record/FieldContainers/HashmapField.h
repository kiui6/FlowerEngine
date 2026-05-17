#pragma once

#include "../FieldBase.h"

#include <unordered_map>

template <typename KeyT, FieldValueClass T>
struct HashmapField
{
    using DecayType = std::unordered_map<KeyT, typename T::DecayType>;

    static void Serialize(const DecayType& data, std::vector<uint8_t>& out) {}
    static void Deserialize(std::span<const uint8_t> data, DecayType& out) {}
};