#pragma once

#include "../FieldBase.h"

#include <string_view>
#include <Localization/Text.h>

struct FText
{
    using DecayType = Text;

    static void Serialize(const DecayType& data, RecordFieldMemory& out) {}
    static void Deserialize(RecordFieldMemory* data, DecayType& out) {}
};