#pragma once

#include "../FieldBase.h"

#include <Log/Log.h>

#include <string_view>

struct FString
{
    using DecayType = std::string;

    static bool Serialize(const DecayType& data, RecordFieldObject::NodeWrapper& out) {return false;}
    static bool Deserialize(const FieldNode& node, DecayType& out) {
        if(node.type != FieldNodeType::String) {
            LOG(Assert, LogFStringDeserialize, "Passed node type mismatch, expected String");
            return false;
        }

        out.assign(node.data.stringValue, node.size);

        return true;
    }
};