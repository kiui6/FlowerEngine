#pragma once

#include "../FieldBase.h"
#include "../RecordPtr.h"

struct FRecord
{
    using DecayType = RecordID;

    static void Serialize(const DecayType& data, RecordObject::NodeWrapper& out) {}
    static void Deserialize(FieldNode* node, DecayType& out) {}
};