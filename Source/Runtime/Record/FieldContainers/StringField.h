#pragma once

#include "../FieldBase.h"

#include <string_view>

struct StringField
{
    using DecayType = std::string;

    static void Serialize(const DecayType& data, std::vector<uint8_t>& out){}
    static void Deserialize(std::span<const uint8_t> data, DecayType& out){}
};