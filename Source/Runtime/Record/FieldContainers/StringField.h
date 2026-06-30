#pragma once

#include "../FieldBase.h"

#include <string_view>

struct FString
{
    using DecayType = std::string;

    static void Serialize(const DecayType& data, RecordFieldObject::NodeWrapper& out) {}
    static void Deserialize(const FieldNode& node, DecayType& out) {
        out.assign(node.data.stringValue, node.size);
    }
};