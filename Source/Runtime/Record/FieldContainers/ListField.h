#pragma once

#include "../FieldBase.h"

#include <Log/Log.h>

#include <vector>
#include <cstdint>
#include <span>

template <FieldValueClass T>
struct FList
{
    using DecayType = std::vector<typename T::DecayType>;

    static bool Serialize(const DecayType& data, RecordFieldObject::NodeWrapper& out) { return false;}
    static bool Deserialize(const FieldNode& node, DecayType& out) {
        if(node.type != FieldNodeType::List) {
            LOG(Assert, LogFListDeserialize, "Passed node type mismatch, expected List");
            return false;
        }

        std::span<FieldNode> children(node.data.children, node.size);
        for(uint32_t i = 0; i < node.size; i++) {
            const FieldNode& element = children[i];

            typename T::DecayType elementData;

            if(!T::Deserialize(element, elementData)) {
                return false;
            }

            out.emplace_back(std::move(elementData));
        }
        return true;
    }
};