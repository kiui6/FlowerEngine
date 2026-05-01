#pragma once

#include "../FieldBase.h"

#include <string_view>
#include <Localization/Text.h>

struct TextField
{
    using DecayType = Text;

    static DecayType DefaultConstructor() {return Text();}

    static void Serialize(const DecayType& data, std::vector<uint8_t>& out){}
    static void Deserialize(std::span<const uint8_t> data, DecayType& out){}
};