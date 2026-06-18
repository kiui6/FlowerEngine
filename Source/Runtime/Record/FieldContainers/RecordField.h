#pragma once

#include "../FieldBase.h"
#include "../RecordPtr.h"

struct FRecord
{
    using DecayType = RecordID;

    static void Serialize(const DecayType& data, RecordFieldMemory& out) {}
    static void Deserialize(RecordFieldMemory* data, DecayType& out) {}
};