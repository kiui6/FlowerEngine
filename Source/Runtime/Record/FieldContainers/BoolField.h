#pragma once

#include "../FieldBase.h"

struct BoolField
{
    using DecayType = bool;

    static DecayType DefaultConstructor() {return false;}

    static void Serialize(const DecayType& data, std::vector<uint8_t>& out) {}
    static void Deserialize(std::span<const uint8_t> data, DecayType& out) {}
};