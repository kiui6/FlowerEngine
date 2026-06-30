#pragma once

#include "../FieldBase.h"
#include "../RecordPtr.h"

struct FRecord
{
    using DecayType = RecordID;

    static void Serialize(const DecayType& data, RecordFieldObject::NodeWrapper& out) {}
    static void Deserialize(const FieldNode& node, DecayType& out) {}
};