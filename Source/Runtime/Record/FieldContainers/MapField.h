#pragma once

#include "../FieldBase.h"

#include <unordered_map>
#include <Utility/Hash.h>

template <FieldValueClass KeyT, FieldValueClass T>
struct FMap
{
    using DecayType = std::unordered_map<typename KeyT::DecayType, typename T::DecayType>;

    static void Serialize(const DecayType& data, RecordObject::NodeWrapper& out) {}
    static void Deserialize(FieldNode* node, DecayType& out) {}
};

template <FieldValueClass T>
struct FMap<FString, T>
{
    using DecayType = std::unordered_map<typename FString::DecayType, typename T::DecayType, StringHash, std::equal_to<>>;

    static void Serialize(const DecayType& data, RecordObject::NodeWrapper& out) {}
    static void Deserialize(FieldNode* node, DecayType& out) {}
};