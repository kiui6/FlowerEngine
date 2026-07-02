#pragma once

#include "../FieldBase.h"

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

template <FieldValueClass KeyT, FieldValueClass ValueT>
struct FMap
{
    using DecayType = std::unordered_map<typename KeyT::DecayType, 
                                typename ValueT::DecayType, 
                                typename FMapDefaultHash<typename KeyT::DecayType>::type,
                                std::equal_to<>>;

    static void Serialize(const DecayType& data, RecordFieldObject::NodeWrapper& out) {}
    static void Deserialize(const FieldNode& node, DecayType& out) {
        assert(node.type == FieldNodeType::Map);

        std::span<FieldNode> children(node.data.children, node.size * 2);
        for(uint32_t i = 0; i < node.size; i++) {
            const FieldNode& key = children[i * 2];
            const FieldNode& value = children[i * 2 + 1];

            typename KeyT::DecayType keyData;
            typename ValueT::DecayType valueData;

            KeyT::Deserialize(key, keyData);
            ValueT::Deserialize(value, valueData);

            out.emplace(keyData, valueData);
        }
    }
};