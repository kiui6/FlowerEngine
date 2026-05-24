#pragma once

#include "../FieldBase.h"
#include "../RecordPtr.h"

// TODO: Make templated type check
struct RecordField
{
    using DecayType = RecordID;

    static void Serialize(const DecayType& data, RecordFieldMemory& out) {}
    static void Deserialize(RecordFieldMemory* data, DecayType& out) {}
};