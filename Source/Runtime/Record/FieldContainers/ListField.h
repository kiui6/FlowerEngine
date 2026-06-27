#pragma once

#include "../FieldBase.h"

#include <vector>

template <FieldValueClass T>
struct FList
{
    using DecayType = std::vector<typename T::DecayType>;

    static void Serialize(const DecayType& data, RecordObject::NodeWrapper& out) {}
    static void Deserialize(FieldNode* node, DecayType& out) {}
};