#pragma once

#include "../FieldBase.h"

#include <Log/Log.h>

#include <vector>

template <FieldValueClass FirstT, FieldValueClass SecondT>
struct FPair
{
    using DecayType = std::pair<typename FirstT::DecayType, typename SecondT::DecayType>;

    static bool Serialize(const DecayType& data, RecordFieldObject::NodeWrapper& out) {}
    static bool Deserialize(const FieldNode& node, DecayType& out) {
        if(node.type != FieldNodeType::Pair) {
            LOG(Assert, LogFPairDeserialize, "Passed node type mismatch, expected Pair");
            return false;
        }

        const FieldNode& first = node.data.children[0];
        const FieldNode& second = node.data.children[1];

        typename FirstT::DecayType firstData;
        typename SecondT::DecayType secondData;

        if(!FirstT::Deserialize(first, firstData) || !SecondT::Deserialize(second, secondData)) {
            return false;
        }

        out = std::make_pair(std::move(firstData), std::move(secondData));

        return true;
    }
};