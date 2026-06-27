#pragma once

#include "../FieldBase.h"

#include <vector>

template <FieldValueClass KeyT, FieldValueClass ValueT>
struct FPair
{
    using DecayType = std::pair<typename KeyT::DecayType, typename ValueT::DecayType>;

    static void Serialize(const DecayType& data, RecordObject::NodeWrapper& out) {}
    static void Deserialize(FieldNode* node, DecayType& out) {}
};