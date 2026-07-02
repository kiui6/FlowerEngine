#pragma once

#include "../FieldBase.h"

#include <Log/Log.h>

#include <unordered_map>
#include <span>
#include <cassert>
#include <Utility/Hash.h>

template <class KeyT>
struct FMapDefaultHash {
    using type = std::hash<KeyT>;
};

template <>
struct FMapDefaultHash<std::string> {
    using type = StringHash;
};

template <FieldValueClass KeyT, FieldValueClass SecondT>
struct FMap
{
    using DecayType = std::unordered_map<typename KeyT::DecayType, 
                                typename SecondT::DecayType, 
                                typename FMapDefaultHash<typename KeyT::DecayType>::type,
                                std::equal_to<>>;

    static bool Serialize(const DecayType& data, RecordFieldObject::NodeWrapper& out) {}
    static bool Deserialize(const FieldNode& node, DecayType& out) {
        if(node.type != FieldNodeType::Map) {
            LOG(Assert, LogFMapDeserialize, "Passed node type mismatch, expected Map");
            return false;
        }

        std::span<FieldNode> children(node.data.children, node.size * 2);
        for(uint32_t i = 0; i < node.size; i++) {
            const FieldNode& key = children[i * 2];
            const FieldNode& value = children[i * 2 + 1];

            typename KeyT::DecayType keyData;
            typename SecondT::DecayType valueData;

            if(!KeyT::Deserialize(key, keyData) || !SecondT::Deserialize(value, valueData)) {
                return false;
            }

            out.emplace(std::move(keyData), std::move(valueData));
        }

        return true;
    }
};