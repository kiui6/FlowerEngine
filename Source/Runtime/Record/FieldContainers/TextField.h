#pragma once

#include "../FieldBase.h"

#include <Log/Log.h>

#include <string_view>
#include <Localization/Text.h>

struct FText
{
    using DecayType = Text;

    static bool Serialize(const DecayType& data, RecordFieldObject::NodeWrapper& out) {return false;}
    static bool Deserialize(const FieldNode& node, DecayType& out) { return false; }
};