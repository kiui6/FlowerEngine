#pragma once

#include "../FieldBase.h"

struct BoolField
{
    using DecayType = bool;

    static void Serialize(const DecayType& data, RecordFieldMemory& out) {}
    static void Deserialize(RecordFieldMemory* data, DecayType& out) {}
};